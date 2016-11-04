#pragma once
#include "regex_ast.h"
class Regex
{
public:
	Regex() = default;
	Regex(wstring re) :regex(re) {};
private:
	vector<node_ptr> nodes;
	wstring regex;
};