#include "stdafx.h"
#include "regex.h"
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

