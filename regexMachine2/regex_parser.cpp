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

	basic-RE::= elementary-RE special-symbol
	special-symbol::= "{" range "}" | "*" | "+" | "?" | ¦Å


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
vector<node_ptr> nodes;

/*
	Helper function
*/
bool match(const char &c)
{
	if (c != regex[index++])
	{
		--index;
		return false;
	}
	return true;
}

void regex_parse(string re)
{
	regex = re;
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


node_ptr set()
{

}

void metacharacter(node_ptr &node)
{
	switch (regex[index])
	{
	case 't':
	case 'n':
	case 'r':
	case 'f':
	case 'd':
	case 'D':
	case 's':
	case 'S':
	case 'w':
	case 'W':
		node = make_shared<CharNode>(regex[index], true);
		nodes.push_back(node);
		break;
	case '|':
	case '.':
	case '*':
	case '?':
	case '+':
	case '[':
	case ']':
	case '{':
	case '}':
	case '\\':
	default:
		node = make_shared<CharNode>(regex[index], false);
		nodes.push_back(node);
		break;
	}
}

node_ptr elementary_re()
{
	node_ptr node = nullptr;
	if (match('('))
	{
		if (match(')'))
			throw runtime_error("Missing argument in '(' ')'");
		else
		{
			node = regular_expression();
			if (!match(')'))
				throw runtime_error("Syntax error:missing ')'");
		}
	} else if (match('\\'))
	{
		metacharacter(node);
	} else if (match('['))
	{
		char c;
		if (match('^'))
		{
			shared_ptr<SetNode> set_n(false);
			while (isalnum(regex[index]) || regex[index] == '_')
			{
				c = regex[index];
				if (match('-'))
				{
					if (c >= regex[index])
						throw runtime_error("Wrong argument in \"[a-b]\"");
					else
						set_n->add_set_range(make_pair(c, regex[index++]));
				} else
					set_n->add_set_range(c);
			}
		} else
		{

		}
	}
}

pair<int, int> range()
{
	int num1 = 0, num2 = 0;
	if (isdigit(regex[index]))
		num1 = regex[index++] - '0';
	else
		throw runtime_error("Missing argument next to the '{'.");
	while (isdigit(regex[index]))
	{
		num1 = num1 * 10 + regex[index] - '0';
		++index;
	}
	if (match('}'))
		return make_pair(num1, num1);
	else if (match(','))
	{
		if (isdigit(regex[index]))
			num2 = regex[index++] - '0';
		else
			throw runtime_error("Missing argument next to the ','.");
		while (isdigit(regex[index]))
		{
			num2 = num2 * 10 + regex[index] - '0';
			++index;
		}
		if (match('}'))
			if (num1 == num2)
				throw runtime_error("In the operation \"{num1,num2}\",num1 is equal to num2.");
			else
				return make_pair(num1, num2);
		else
			throw runtime_error("Syntax error:missing '}'.");
	} else
		throw runtime_error("Syntax error:wrong symbol in the operation \"{num1,num2}\".");
}


node_ptr basic_re()
{
	node_ptr ele = elementary_re();
	node_ptr temp = nullptr;
	if (match('{'))
	{
		if (match('}'))
			throw runtime_error("Missing argument in '{' '}'");
		pair<int, int> p = range();
		temp = make_shared<RangeNode>(ele, p.first, p.second);
		nodes.push_back(temp);
	} else if (match('*'))
	{
		temp = make_shared<StarNode>(ele);
		nodes.push_back(temp);
	} else if (match('+'))
	{
		temp = make_shared<PlusNode>(ele);
		nodes.push_back(temp);
	} else if (match('?'))
	{
		temp = make_shared<QuesNode>(ele);
		nodes.push_back(temp);
	}
	return temp;
}

node_ptr _union()
{
	if (index == regex.size())
		return nullptr;

	if (!match('|'))
		throw runtime_error("Syntax error:missing '|'");
	node_ptr left = nullptr, right = nullptr, union_node = nullptr;
	left = simple_re();
	right = _union();
	if (right == nullptr)
		return left;
	else
	{
		union_node = make_shared<AlternationNode>(left, right);
		nodes.push_back(union_node);
		return union_node;
	}

}

node_ptr concatenation()
{
	node_ptr left = nullptr, right = nullptr, con_node = nullptr;
	left = basic_re();
	if (left == nullptr)
		return nullptr;
	right = concatenation();
	if (right == nullptr)
		return left;
	else
	{
		con_node = make_shared<ConcatenationNode>(left, right);
		nodes.push_back(con_node);
		return con_node;
	}
}

node_ptr simple_re()
{
	node_ptr left = nullptr, right = nullptr, con_node = nullptr;
	left = basic_re();
	right = concatenation();
	if (right == nullptr)
		return left;
	else
	{
		con_node = make_shared<ConcatenationNode>(left, right);
		nodes.push_back(con_node);
		return con_node;
	}
}

node_ptr regular_expression()
{
	node_ptr left = nullptr, right = nullptr, union_node = nullptr;
	left = simple_re();
	right = _union();
	if (right == nullptr)
		return left;
	else
	{
		union_node = make_shared<AlternationNode>(left, right);
		nodes.push_back(union_node);
		return union_node;
	}
}
