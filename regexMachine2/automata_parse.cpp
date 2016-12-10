#include "stdafx.h"
#include "automata.h"
#include "regex.h"
#include "visitor.h"
#include "regex_ast.h"
#include <set>
#include <list>
#include <tuple>
#include <map>


namespace regex_engine2_parser {

using regex_engine2_ast::node_ptr;
regex_engine2_ast::AST regex_parse(std::wstring re);

}

namespace regex_engine2_automata_parse {

using regex_engine2_automata::Automata;
using regex_engine2_automata::status_ptr;
using regex_engine2_automata::edge_ptr;
using regex_engine2_automata::DFAStatus;

using regex_engine2_automata::index_set;
using regex_engine2_automata::status_set;
using regex_engine2_automata::Dtran;
using regex_engine2_automata::dfa_index;
using regex_engine2_automata::nfa_index;

using regex_engine2_regex::group_index;
using regex_engine2_regex::CharSet;
using regex_engine2_ast::AST;

using regex_engine2_visitor::NFAConstructVisitor;
using regex_engine2_visitor::CharSetConstructVisitor;

using regex_engine2_parser::regex_parse;


using std::vector;
using std::set;
using std::list;
using std::map;
using std::tuple;
using std::wstring;


namespace {



std::vector<status_ptr> all_status;

inline index_set switch_to_i(status_set &s)
{
	index_set set;
	for (auto &k : s)
		set.insert(k->get_index());
	return set;
}

inline status_set switch_to_s(index_set &i)
{
	status_set set;
	for (auto &k : i)
		set.push_back(all_status[k]);
	return set;
}

index_set epsilon_closure(status_ptr &s)
{
	status_set set;
	index_set result;
	set.push_back(s);
	result.insert(s->get_index());
	for (auto it = set.begin(); it != set.end(); ++it)
	{
		for (auto &e : (*it)->get_out_edges())
		{
			if (e->get_match_content().empty())
			{
				set.push_back(e->get_end());
				result.insert((e->get_end())->get_index());
			}
		}
	}
	return result;
}


index_set epsilon_closure(index_set s)
{
	index_set set;
	index_set set_t;
	if (s.empty())
		return set;
	status_set temp = switch_to_s(s);
	for (auto &i : temp)
	{
		set_t = epsilon_closure(i);
		for (auto &j : set_t)
			set.insert(j);
	}
	return set;
}

index_set move(index_set &set, group_index a)
{
	status_set s_set = switch_to_s(set);
	index_set s;
	vector<group_index> content;
	vector<group_index>::iterator it;
	for (auto &i : s_set)
	{
		for (auto &e : i->get_out_edges())
		{
			content = e->get_match_content();
			if (!content.empty())
			{
				it = std::find(content.begin(), content.end(), a);
				if (it != content.end())
					s.insert((e->get_end())->get_index());
			}
		}
	}
	return s;
}


Dtran dfa_minimize(list<DFAStatus> &tran)
{
	auto capacity = DFAStatus::size();
	list<DFAStatus*> final_status, non_final_status;
	for (auto &status : tran)
	{
		if (status.is_final())
			final_status.push_back(&status);
		else
			non_final_status.push_back(&status);
	}
	list<list<DFAStatus*>> set_group;
	if (non_final_status.empty())
		set_group = { final_status };
	else
		set_group = { non_final_status,final_status };
	list<DFAStatus*> *target_set = nullptr;
	/************************************************************************/
	/*                    Lambda Functions                                  */
	/************************************************************************/
	auto find_target = [&set_group](dfa_index index)->list<DFAStatus*>* {
		if (index == -1)
			return nullptr;
		for (auto &set : set_group)
		{
			for (auto &dfa : set)
			{
				if (dfa->get_index() == index)
					return &set;
			}
		}
		return nullptr;
	};

	auto in_same_set = [&target_set](dfa_index index)->bool {
		if (!target_set&&index == -1)
			return true;
		else if (target_set&&index != -1)
		{
			for (auto dfa : *target_set)
			{
				if (dfa->get_index() == index)
					return true;
			}
		}
		return false;
	};
	/************************************************************************/
	/*                   Lambda Functions                                   */
	/************************************************************************/
	vector<list<DFAStatus*>::iterator> dfa_to_delete;
	DFAStatus *first_dfa;
	list<DFAStatus*> new_set;
	size_t old_size;
	for (;;)
	{
		old_size = set_group.size();
		for (auto it1 = set_group.begin(); it1 != set_group.end(); ++it1)
		{
			if (!new_set.empty())
			{
				set_group.insert(it1, new_set);
				new_set.clear();
			}
			if (it1->size() == 1)
				continue;
			first_dfa = *(it1->begin());
			for (unsigned int i = 0; i < capacity; ++i)
			{
				target_set = find_target((*first_dfa)[i]);
				auto it2 = it1->begin();
				it2++;
				for (; it2 != it1->end(); ++it2)
				{
					if (!in_same_set((*(*it2))[i]))
					{
						if (!dfa_to_delete.empty())
						{
							auto pos = std::find(dfa_to_delete.begin(), dfa_to_delete.end(), it2);
							if (pos == dfa_to_delete.end())
							{
								new_set.push_back(*it2);
								dfa_to_delete.push_back(it2);
							}
						} else
						{
							new_set.push_back(*it2);
							dfa_to_delete.push_back(it2);
						}
					}
				}
			}
			for (auto it : dfa_to_delete)
				it1->erase(it);
			dfa_to_delete.clear();
		}
		if (!new_set.empty())
		{
			set_group.push_back(new_set);
			new_set.clear();
		}
		if (old_size == set_group.size())
			break;
	}

	Dtran mini_tran;
	map<dfa_index, dfa_index> new_dtrans;
	new_dtrans[-1] = -1;
	dfa_index new_index = 0;
	for (auto &set_t : set_group)
	{
		for (auto &dfa : set_t)
		{
			new_dtrans[dfa->get_index()] = new_index;
		}
		++new_index;
	}

	for (auto &set : set_group)
	{
		auto temp_s = set.begin();
		for (unsigned int i = 0; i < capacity; ++i)
			(*(*temp_s))[i] = new_dtrans[(*(*temp_s))[i]];
		mini_tran.push_back(*(*temp_s));
	}

	return mini_tran;
}

list<DFAStatus> nfa_to_dfa(Automata &nfa, CharSet &char_set)
{
	DFAStatus::reset();
	DFAStatus::set_size(char_set.size());
	all_status = nfa.all_status;
	list<DFAStatus> Dstatus;
	//Dtran tran;
	auto temp = epsilon_closure(nfa.start);
	assert(temp.size() > 0);
	auto it = temp.find(nfa.get_final_index());
	if (it != temp.end())
		Dstatus.push_back(DFAStatus(temp, true));
	else
		Dstatus.push_back(DFAStatus(temp));

	auto add = [&Dstatus](index_set &is, bool final)->dfa_index {
		dfa_index i = 0;
		for (auto &ss : Dstatus)
		{
			if (ss.get_nfa_set() == is)
				return i;
			++i;
		}
		Dstatus.push_back(DFAStatus(is, final));
		return i;
	};

	bool final = false;
	set<nfa_index> temp2;
	set<nfa_index>::iterator itt;
	for (auto it = Dstatus.begin(); it != Dstatus.end(); ++it)
	{
		for (group_index i = 0; i < char_set.size(); ++i)
		{
			temp2 = epsilon_closure(move(it->get_nfa_set(), i));
			if (!temp2.empty())
			{
				itt = temp2.find(nfa.get_final_index());
				if (itt != temp2.end())
					final = true;
				//it->set_tran(i, add(temp2, final));
				(*it)[i] = add(temp2, final);
				final = false;
			}
		}
	}
	all_status.clear();
	return Dstatus;
}

Automata nodes_to_nfa(AST &ast, CharSet &set)
{
	NFAConstructVisitor nfa_visitor(set);
	return nfa_visitor.invoke(ast.get_root());
}

}


tuple<CharSet, Dtran> automata_parse(wstring restring)
{
	AST ast = regex_parse(restring);
	CharSetConstructVisitor e_visitor;
	for (auto &n : *(ast.get_nodes()))
		n->accept_visitor(&e_visitor);
	auto c_set = std::move(e_visitor.get_set());
	Automata nfa = nodes_to_nfa(ast, c_set);
	auto dtran = nfa_to_dfa(nfa, c_set);
	nfa.free();
	auto mini_dtran = dfa_minimize(dtran);

	return std::make_tuple(c_set, mini_dtran);
}


}

