#include "stdafx.h"
#include "regex.h"

using std::vector;
using std::wstring;

vector<node_ptr> *regex_parse(wstring re);

void parse(wstring regex)
{
	vector<node_ptr> *nodes = regex_parse(std::move(regex));

}


Regex::Regex(wstring re)
{
	set_regex(std::move(re));
}

void Regex::set_regex(wstring re)
{
	regex = std::move(re);
	parse(regex);
}

bool Regex::match(wstring teststring)
{
	return false;
}

EdgeSet & EdgeSet::add_edge(edge)
{
	// TODO: 在此处插入 return 语句
}

std::vector<int> EdgeSet::get_edge_index(edge)
{
	return std::vector<int>();
}

int EdgeSet::get_edge_index(wchar_t)
{
	return 0;
}

edge & EdgeSet::get_edge(int index)
{
	// TODO: 在此处插入 return 语句
}
