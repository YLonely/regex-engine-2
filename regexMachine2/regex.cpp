#include "stdafx.h"
#include "regex.h"
#include "regex_parse.h"
#include "visitor.h"
#include "automata.h"
#include <set>
#include <list>

namespace regex_engine2_regex {

using std::vector;
using std::wstring;
using regex_engine2_parser::regex_parse;
using regex_engine2_ast::node_ptr;
using regex_engine2_ast::AST;
using regex_engine2_visitor::EdgeSetConstructVisitor;
using regex_engine2_visitor::NFAConstructVisitor;
using regex_engine2_automata::Automata;
using regex_engine2_automata::status_ptr;
using regex_engine2_automata::edge_ptr;
using regex_engine2_automata::DFAStatus;

using std::pair;
using std::make_pair;
using std::set;
using std::list;

typedef list<status_ptr> status_set;//status set
typedef std::set<int> index_set;//index set
typedef vector<DFAStatus> Dtran;


namespace {
std::vector<status_ptr> all_status;

index_set switch_to_i(status_set s)
{
	index_set set;
	for (auto &k : s)
		set.insert(k->get_index());
	return set;
}

status_set switch_to_s(index_set i)
{
	status_set set;
	for (auto &k : i)
		set.push_back(all_status[k]);
	return set;
}



status_set epsilon_closure(status_ptr s)
{
	status_set set;
	set.push_back(s);
	for (auto it = set.begin(); it != set.end(); ++it)
	{
		for (auto &e : (*it)->get_out_edges())
		{
			if ((e->get_match_content()).size() == 0)
				set.push_back(e->get_end());
		}
	}
	return set;
}


status_set epsilon_closure(status_set s)
{
	index_set set;
	index_set temp;
	for (auto &k : s)
	{
		temp = switch_to_i(epsilon_closure(k));
		for (auto &t : temp)
			set.insert(t);
	}
	return switch_to_s(set);
}

status_set move(status_set set, index_t a)
{
	status_set s;
	for (auto &i : set)
	{
		for (auto &e : i->get_out_edges())
		{
			auto it = std::find(e->get_match_content().begin(), e->get_match_content().end(), a);
			if (it != e->get_match_content().end())
				s.push_back(e->get_end());
		}
	}
}


void dfa_minimize(Automata &dfa)
{

}

Dtran nfa_to_dfa(Automata &nfa, EdgeSet &set)
{
	all_status = nfa.all_status;
	list<index_set> Dstatus;
	Dstatus.push_back(switch_to_i(epsilon_closure(nfa.start)));
	for (auto it = Dstatus.begin(); it != Dstatus.end(); ++it)
	{

	}


	all_status.clear();
}

Automata nodes_to_nfa(AST &ast, EdgeSet &set)
{
	NFAConstructVisitor nfa_visitor(set);
	return nfa_visitor.invoke(ast.get_root());
}

}

void parse(Regex &re)
{
	AST ast = regex_parse(re.regex);
	EdgeSetConstructVisitor e_visitor;
	for (auto &n : *(ast.get_nodes()))
		n->accept_visitor(&e_visitor);
	re.set = std::move(e_visitor.get_set());
	Automata nfa = nodes_to_nfa(ast, re.set);

	ast.release_nodes();//Release the memory of ast nodes.

	nfa_to_dfa(nfa, re.set);
}

void Regex::set_regex(wstring re)
{
	regex = std::move(re);
	parse(*this);
}

bool Regex::match(wstring teststring)
{
	return false;
}

EdgeSet & EdgeSet::add_edge(edge e)
{
	if (e_set.empty())
		e_set.push_back(std::move(e));
	else
	{
		vector<edge> temp;
		for (auto &it = e_set.begin(); it != e_set.end(); ++it)
		{
			if (e.second < it->first)
			{
				temp.push_back(std::move(e));
				temp.insert(temp.end(), it, e_set.end());
				break;
			} else if (e.first <= it->first&&e.second >= it->first&&e.second <= it->second)
			{
				if (e.first == it->first&&e.second == it->second)
				{
					temp.insert(temp.end(), it, e_set.end());
					break;
				} else if (e.first == it->first)
				{
					temp.push_back({ e.first,e.second });
					temp.push_back({ e.second + 1,it->second });
				} else if (e.second == it->second)
				{
					temp.push_back({ e.first,it->first - 1 });
					temp.push_back({ it->first,it->second });
				} else
				{
					temp.push_back({ e.first,it->first - 1 });
					temp.push_back({ it->first,e.second });
					temp.push_back({ e.second + 1,it->second });
				}
				temp.insert(temp.end(), it + 1, e_set.end());
				break;
			} else if (e.first <= it->first&&e.second > it->second)
			{
				if (e.first == it->first)
				{
					temp.push_back({ it->first,it->second });
				} else
				{
					temp.push_back({ e.first,it->first - 1 });
					temp.push_back({ it->first,it->second });
				}
				e.first = it->second + 1;
			} else if (e.second <= it->second&&e.first > it->first)
			{
				if (e.second == it->second)
				{
					temp.push_back({ it->first,e.first - 1 });
					temp.push_back({ e.first,e.second });
				} else
				{
					temp.push_back({ it->first,e.first - 1 });
					temp.push_back({ e.first,e.second });
					temp.push_back({ e.second + 1,it->second });
				}
				temp.insert(temp.end(), it + 1, e_set.end());
				break;
			} else if (e.second > it->second&&e.first >= it->first&&e.first <= it->second)
			{
				if (e.first == it->first)
				{
					temp.push_back({ it->first,it->second });
				} else if (e.first == it->second)
				{
					temp.push_back({ it->first,e.first - 1 });
					temp.push_back({ e.first,e.first });
				} else
				{
					temp.push_back({ it->first,e.first - 1 });
					temp.push_back({ e.first,it->second });
				}
				e.first = it->second + 1;
			} else
				temp.push_back(*it);
			if (it + 1 == e_set.end())
			{
				temp.push_back(std::move(e));
				break;
			}
		}
		e_set = std::move(temp);
	}
	return *this;
}

std::vector<index_t> EdgeSet::get_edge_index(edge e)
{
	vector<index_t> temp;
	bool state = false;
	for (index_t i = 0; i < e_set.size(); ++i)
	{
		if (!state&&e.first == e_set[i].first)
		{
			state = true;
			temp.push_back(i);
			if (e.second == e_set[i].second)
			{
				state = false;
				break;
			}
		} else if (state&&e.second == e_set[i].second)
		{
			state = false;
			temp.push_back(i);
		} else if (state)
			temp.push_back(i);
	}
	if (state)
		return{};
	return temp;
}

/*
	edges must be non-intersect and only be used for SetNode.
*/
std::vector<index_t> EdgeSet::get_edge_index(std::vector<edge> edges)
{
	vector<index_t> result;
	vector<index_t> temp;
	for (auto &e : edges)
	{
		temp = std::move(get_edge_index(e));
		for (auto &i : temp)
			result.push_back(i);
	}
	return result;
}

index_t EdgeSet::get_edge_index(wchar_t &ch)
{
	index_t index = e_set.size();
	if (e_set.empty())
		return index;
	for (index_t i = 0; i < e_set.size(); ++i)
	{
		if (ch >= e_set[i].first&&ch <= e_set[i].second)
		{
			index = i;
			break;
		}
	}
	return index;
}

edge EdgeSet::get_edge(index_t index)
{
	if (index >= e_set.size())
		throw std::runtime_error("Get edge out of range.");
	return e_set[index];
}


}