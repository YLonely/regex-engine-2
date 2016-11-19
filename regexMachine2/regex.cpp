#include "stdafx.h"
#include "regex.h"
#include "regex_parse.h"
#include "visitor.h"
#include "automata.h"

namespace regex_engine2_regex {

using std::vector;
using std::wstring;
using regex_engine2_parser::regex_parse;
using regex_engine2_astnode::node_ptr;
using regex_engine2_visitor::EdgeSetConstructVisitor;
using regex_engine2_automata::Automata;

using std::pair;
using std::make_pair;

void dfa_minimize(Automata &dfa)
{

}

void nfa_to_dfa(Automata &nfa)
{

}

void nodes_to_nfa(vector<node_ptr> &nodes)
{

}

void parse(Regex &re)
{
	vector<node_ptr> *nodes = regex_parse(re.regex);
	EdgeSetConstructVisitor e_visitor;
	for (auto &n : *nodes)
		n->accept_visitor(e_visitor);
	re.set = std::move(e_visitor.get_set());
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

index_t EdgeSet::get_edge_index(wchar_t ch)
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