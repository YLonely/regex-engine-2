#include "stdafx.h"
#include "automata.h"
#include "regex.h"
#include "visitor.h"
#include "regex_ast.h"
#include <set>
#include <list>
#include <tuple>


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

using regex_engine2_regex::index_t;
using regex_engine2_regex::EdgeSet;
using regex_engine2_ast::AST;

using regex_engine2_visitor::NFAConstructVisitor;
using regex_engine2_visitor::EdgeSetConstructVisitor;

using regex_engine2_parser::regex_parse;


using std::vector;
using std::set;
using std::list;
using std::tuple;
using std::wstring;


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



index_set epsilon_closure(status_ptr s)
{
	status_set set;
	index_set result;
	set.push_back(s);
	result.insert(s->get_index());
	for (auto it = set.begin(); it != set.end(); ++it)
	{
		for (auto &e : (*it)->get_out_edges())
		{
			if ((e->get_match_content()).size() == 0)
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
	status_set temp = switch_to_s(s);
	for (auto &i : temp)
	{
		set_t = epsilon_closure(i);
		for (auto &j : set_t)
			set.insert(j);
	}
	return set;
}

index_set move(index_set set, index_t a)
{
	status_set s_set = switch_to_s(set);
	index_set s;
	for (auto &i : s_set)
	{
		for (auto &e : i->get_out_edges())
		{
			if (e->get_match_content().size())
			{
				auto it = std::find(e->get_match_content().begin(), e->get_match_content().end(), a);
				if (it != e->get_match_content().end())
					s.insert((e->get_end())->get_index());
			}
		}
	}
	return s;
}


void dfa_minimize(Automata &dfa)
{

}

Dtran nfa_to_dfa(Automata &nfa, EdgeSet &set)
{
	all_status = nfa.all_status;
	list<DFAStatus> Dstatus;
	Dtran tran;
	Dstatus.push_back(DFAStatus(set.get_capacity(), epsilon_closure(nfa.start)));

	auto add = [&Dstatus](DFAStatus s)->int {
		int i = 0;
		for (auto &ss : Dstatus)
		{
			if (ss.get_nfa_set() == s.get_nfa_set())
				return i;
			++i;
		}
		Dstatus.push_back(s);
		return i;
	};
	bool final = false;
	for (auto it = Dstatus.begin(); it != Dstatus.end(); ++it)
	{
		for (index_t i = 0; i < set.get_capacity(); ++i)
		{
			auto temp = epsilon_closure(move(it->get_nfa_set(), i));
			if (temp.size() != 0)
			{
				auto itt = find(temp.begin(), temp.end(), nfa.get_final_index());
				if (itt != temp.end())
					final = true;
				it->set_tran(i, add(DFAStatus(set.get_capacity(), temp, final)));
			} else
				it->set_tran(i, -1);
		}
	}

	for (auto &dfa : Dstatus)
		tran.push_back(dfa);

	all_status.clear();
	return tran;
}

Automata nodes_to_nfa(AST &ast, EdgeSet &set)
{
	NFAConstructVisitor nfa_visitor(set);
	return nfa_visitor.invoke(ast.get_root());
}

}


tuple<EdgeSet, Dtran> automata_parse(wstring restring)
{
	AST ast = regex_parse(restring);
	EdgeSetConstructVisitor e_visitor;
	for (auto &n : *(ast.get_nodes()))
		n->accept_visitor(&e_visitor);
	auto set = std::move(e_visitor.get_set());
	Automata nfa = nodes_to_nfa(ast, set);

	ast.release_nodes();//Release the memory of ast nodes.

	auto tran = nfa_to_dfa(nfa, set);
	return make_tuple(set, tran);
}


}

