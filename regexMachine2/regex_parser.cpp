#include "stdafx.h"

/*
	The parser for regex.Transform the regex into ast.

	RE::= simple-RE union
	union::= "|" simple-RE union | ¦Å
	simple-RE::= basic-RE concatenation
	concatenation::= basic-RE concatenation | ¦Å
	basic-RE::=	num-range | star | plus | ques | elementary-RE
	star::=	elementary-RE "*"
	plus::=	elementary-RE "+"
	ques::=	elementary-RE "?"
	num-range::= elementary-RE "{" range "}"
	range::= num | num "," | num "," num
	elementary-RE::= group | any | eos | char | set
	group::= "(" RE ")"
	any::= "."
	eos::= "$"  //end of string
	char::= any-non-metacharacter | "\" metacharacter
	set::= positive-set | negative-set
	positive-set::= "[" set-items "]"
	negative-set::= "[^" set-items "]"
	set-items::= set-item | set-item set-items
	set-item::= char-range | char
	char-range::= char "-" char
*/



static string::size_type index = 0; //the index of regex string.
static string regex;

/*
	Helper function
*/
bool match(const string &s)
{
	string::size_type temp = index;
	for (auto &c : s)
	{
		if (c != regex[index++])
		{
			index = temp;
			return false;
		}
	}
	return true;
}

void regex_parse(string re)
{
	regex = re;
}





node_ptr _union()
{

}

node_ptr basic_re()
{

}

node_ptr concatenation()
{

}

node_ptr num_range()
{

}

node_ptr star()
{

}

node_ptr plus()
{

}

node_ptr ques()
{

}

node_ptr elementary_re()
{

}

node_ptr set()
{

}

node_ptr simple_re()
{
	node_ptr left = nullptr, right = nullptr;
	left = basic_re();
	right = concatenation();
	if (right == nullptr)
		return left;
	else
		return make_shared<IASTNode>(left, right);
}

node_ptr regular_expression()
{
	node_ptr left = nullptr, right = nullptr;
	left = simple_re();
	right = _union();
	if (right == nullptr)
		return left;
	else
		return make_shared<IASTNode>(left, right);
}
