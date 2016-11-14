#include "stdafx.h"
#include "regex.h"
#include "regex_parse.h"

namespace regex_engine2_regex {

using std::vector;
using std::wstring;
using regex_engine2_parser::regex_parse;
using regex_engine2_astnode::node_ptr;

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

//EdgeSet & EdgeSet::add_edge(edge)
//{
//	// TODO: 在此处插入 return 语句
//}

std::vector<int> EdgeSet::get_edge_index(edge)
{
	return std::vector<int>();
}

int EdgeSet::get_edge_index(wchar_t)
{
	return 0;
}

//edge & EdgeSet::get_edge(int index)
//{
//	// TODO: 在此处插入 return 语句
//}


}
