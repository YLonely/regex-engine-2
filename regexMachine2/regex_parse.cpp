#include "stdafx.h"
#include "regex_parse.h"
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

namespace regex_engine2_parser {

using std::pair;
using std::vector;
using std::wstring;
using std::shared_ptr;
using std::runtime_error;

using std::make_shared;
using std::make_pair;

using namespace regex_engine2_ast;

namespace {

wstring::size_type index; //the index of regex string.
wstring regex;
std::unique_ptr<vector<node_ptr>> nodes;

node_ptr regular_expression();
node_ptr simple_re();


/*
	Helper function
*/
bool match(const wchar_t &c)
{
	if (index < regex.size() && c == regex[index])
	{
		++index;
		return true;
	}
	return false;
}

bool is_metachar(const wchar_t &c)
{
	switch (c)
	{
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
	case '(':
	case ')':
		return true;
	default:
		break;
	}
	return false;
};


void set_item(shared_ptr<SetNode> &set)
{
	wchar_t c1, c2;
	bool setchar = false;
	while (!is_metachar(regex[index]) || regex[index] == '\\')
	{
		if (regex[index] == '\\')
		{
			++index;
			switch (regex[index])
			{
			case 't':
				c1 = '\t';
				break;
			case 'n':
				c1 = '\n';
				break;
			case 'r':
				c1 = '\r';
				break;
			case 'f':
				c1 = '\f';
				break;
			case 'd':
				set->add_set_range(L'0', L'9');
				setchar = true;
				break;
			case 'D':
				set->add_set_range((wchar_t)0, '0' - 1).add_set_range('9' + 1, (wchar_t)65535);
				setchar = true;
				break;
			case 's':
				set->add_set_range(L'\t').add_set_range(L'\n').add_set_range(L'\r').add_set_range(L'\f');
				setchar = true;
				break;
			case 'S':
				set->add_set_range((wchar_t)0, (wchar_t)8).add_set_range((wchar_t)11)
					.add_set_range((wchar_t)14, (wchar_t)65535);
				setchar = true;
				break;
			case 'w':
				set->add_set_range(L'a', L'z').add_set_range(L'A', L'Z').add_set_range(L'0', L'9').add_set_range(L'_');
				setchar = true;
				break;
			case 'W':
				set->add_set_range((wchar_t)0, '0' - 1).add_set_range('9' + 1, 'A' - 1)
					.add_set_range('Z' + 1, (wchar_t)94)
					.add_set_range((wchar_t)96).add_set_range((wchar_t)123, (wchar_t)65535);
				setchar = true;
				break;
			default:
				c1 = regex[index];
				break;
			}
		} else
			c1 = regex[index];
		++index;
		if (match('-'))
		{
			if (setchar)
				throw runtime_error("Wrong argument in \"[a-b]\"");
			if (regex[index] == '\\')
			{
				c2 = regex[++index];
				if (c2 == 'd' || c2 == 'D' || c2 == 's' || c2 == 'S' || c2 == 'w' || c2 == 'W')
					throw runtime_error("Wrong argument in \"[a-b]\"");
				switch (c2)
				{
				case 't':
					c2 = '\t';
					break;
				case 'n':
					c2 = '\n';
					break;
				case 'r':
					c2 = '\r';
					break;
				case 'f':
					c2 = '\f';
					break;
				default:
					break;
				}
			} else
				c2 = regex[index];
			if (c1 >= c2)
				throw runtime_error("Wrong argument in \"[a-b]\"");
			else
				set->add_set_range(c1, c2);
			++index;
		} else if (!setchar)
		{
			set->add_set_range(c1);
			setchar = false;
		}
	}
}



void _char(node_ptr &node)
{
	if (index >= regex.size())
		return;



	if (!is_metachar(regex[index]))
	{
		node = make_shared<CharNode>(regex[index++]);
		nodes->push_back(node);
	}
}


void functional_char(node_ptr &node)
{
	shared_ptr<SetNode> set;
	wchar_t ch = regex[index];
	switch (ch)
	{
	case 't':
		node = make_shared<CharNode>('\t');
		break;
	case 'n':
		node = make_shared<CharNode>('\n');
		break;
	case 'r':
		node = make_shared<CharNode>('\r');
		break;
	case 'f':
		node = make_shared<CharNode>('\f');
		break;
	case 'd':
	case 'D':
		set = ch == 'd' ? make_shared<SetNode>(true) : make_shared<SetNode>(false);
		set->add_set_range(L'0', L'9');
		node = set;
		break;
	case 's':
	case 'S':
		set = ch == 's' ? make_shared<SetNode>(true) : make_shared<SetNode>(false);
		set->add_set_range(L'\t').add_set_range(L'\n').add_set_range(L'\r').add_set_range(L'\f');
		node = set;
		break;
	case 'w':
	case 'W':
		set = ch == 'w' ? make_shared<SetNode>(true) : make_shared<SetNode>(false);
		set->add_set_range(L'a', L'z').add_set_range(L'A', L'Z').add_set_range(L'0', L'9').add_set_range(L'_');
		node = set;
		break;
	default:
		break;
	}
}


node_ptr elementary_re()
{


	/*
		Special char is:
		\t {=tab character}
		\n {=newline character}
		\r {=carriage return character}
		\f {=form feed character}
		\d {=a digit, [0-9]}
		\D {=not a digit, [^0-9]}
		\s {=whitespace, [\t\n\r\f]}
		\S {=not a whitespace, [^\t\n\r\f]}
		\w {='word' character, [a-zA-Z0-9_]}
		\W {=not a 'word' character, [^a-zA-Z0-9_]}
	*/
	auto is_functionalchar = [](const wchar_t &c)->bool {switch (c)
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
		return true;
	default:
		break;
	}
	return false;
	};

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
		if (is_functionalchar(regex[index]))
		{
			functional_char(node);
			++index;
		} else
			node = make_shared<CharNode>(regex[index++]);
		nodes->push_back(node);
	} else if (match('['))
	{
		if (match(']'))
			throw runtime_error("Missing argument in '[' ']'");
		shared_ptr<SetNode> set_n;
		if (match('^'))
		{
			set_n = make_shared<SetNode>(false);
			set_item(set_n);
		} else
		{
			set_n = make_shared<SetNode>(true);
			set_item(set_n);
		}
		if (!match(']'))
			throw runtime_error("Syntax error:missing ']'");
		node = set_n;
		set_n->merge();
		nodes->push_back(node);
	} else if (match('.'))
	{
		auto n = make_shared<SetNode>(true);
		n->add_set_range((wchar_t)0, (wchar_t)65535);
		nodes->push_back(n);
	} else
		_char(node);

	return node;
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
			num2 = -1;
		while (isdigit(regex[index]))
		{
			num2 = num2 * 10 + regex[index] - '0';
			++index;
		}
		if (match('}'))
			if (num1 >= num2&&num2 != -1)
				throw runtime_error("In the operation \"{num1,num2}\",num1 must be smaller than num2.");
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
		nodes->push_back(temp);
	} else if (match('*'))
	{
		temp = make_shared<StarNode>(ele);
		nodes->push_back(temp);
	} else if (match('+'))
	{
		temp = make_shared<PlusNode>(ele);
		nodes->push_back(temp);
	} else if (match('?'))
	{
		temp = make_shared<QuesNode>(ele);
		nodes->push_back(temp);
	} else
		temp = ele;
	return temp;
}

node_ptr _union()
{
	/*if (index == regex.size())
		return nullptr;*/
	if (!match('|'))
		return nullptr;
	node_ptr left = nullptr, right = nullptr, union_node = nullptr;
	left = simple_re();
	right = _union();
	if (right == nullptr)
		return left;
	else
	{
		union_node = make_shared<AlternationNode>(left, right);
		nodes->push_back(union_node);
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
		nodes->push_back(con_node);
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
		nodes->push_back(con_node);
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
		nodes->push_back(union_node);
		return union_node;
	}
}

}

AST regex_parse(wstring re)
{
	regex = std::move(re);
	nodes.reset(new vector<node_ptr>());
	index = 0;
	regular_expression();
	nodes->push_back(make_shared<EndOfString>(nodes->back()));
	AST ast(nodes.release());
	return ast;
}

}
