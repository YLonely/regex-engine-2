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

IASTNode *regular_expression()
{

}

IASTNode *simple_re()
{

}

IASTNode *_union()
{

}

IASTNode *basic_re()
{

}

IASTNode *concatenation()
{

}

IASTNode *num_range()
{

}

IASTNode *star()
{

}

IASTNode *plus()
{

}

IASTNode *ques()
{

}

IASTNode *elementary_re()
{

}

IASTNode *set()
{

}

