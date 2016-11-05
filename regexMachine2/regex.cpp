#include "stdafx.h"
#include "regex.h"
vector<node_ptr> *regex_parse(wstring re);

void parse(wstring regex)
{
	vector<node_ptr> *nodes = regex_parse(std::move(regex));

}


bool Regex::match(wstring teststring)
{
	parse(teststring);
	return false;
}

