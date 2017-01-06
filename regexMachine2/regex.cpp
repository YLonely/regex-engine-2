#include "stdafx.h"
#include "regex.h"
#include "visitor.h"
#include "automata.h"
#include "regex_exception.h"
#include <tuple>


namespace regex_engine2_automata_parse {
std::tuple<regex_engine2_regex::CharSet, regex_engine2_automata::Dtran> automata_parse(std::wstring restring);
}


namespace regex_engine2_regex {

using std::vector;
using std::wstring;

using std::pair;
using std::make_pair;
using std::tuple;
using regex_engine2_exception::regex_runtime_error;
using regex_engine2_automata_parse::automata_parse;


void parse(Regex &re)
{
	auto result = automata_parse(re.regex);
	re.set = std::move(std::get<0>(result));
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

CharSet & CharSet::add_group(char_group e)
{
	if (e_set.empty())
		e_set.push_back(std::move(e));
	else
	{
		vector<char_group> temp;
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

std::vector<group_index> CharSet::get_group_index(char_group e)
{
	vector<group_index> temp;
	bool state = false;
	for (group_index i = 0; i < e_set.size(); ++i)
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
std::vector<group_index> CharSet::get_group_index(std::vector<char_group> edges)
{
	vector<group_index> result;
	vector<group_index> temp;
	for (auto &e : edges)
	{
		temp = std::move(get_group_index(e));
		for (auto &i : temp)
			result.push_back(i);
	}
	return result;
}

group_index CharSet::get_group_index(wchar_t &ch)
{
	group_index index = e_set.size();
	if (e_set.empty())
		return index;
	for (group_index i = 0; i < e_set.size(); ++i)
	{
		if (ch >= e_set[i].first&&ch <= e_set[i].second)
		{
			index = i;
			break;
		}
	}
	return index;
}

char_group CharSet::get_group(group_index index)
{
	if (index >= e_set.size())
		throw regex_runtime_error(L"Get edge out of range.");
	return e_set[index];
}


}