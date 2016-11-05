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



static wstring::size_type index = 0; //the index of regex string.
static wstring regex;
static vector<node_ptr> *nodes;

node_ptr regular_expression();
node_ptr simple_re();


/*
	Helper function
*/
static bool match(const wchar_t &c)
{
	if (index < regex.size() && c == regex[index])
	{
		++index;
		return true;
	}
	return false;
}


vector<node_ptr> *regex_parse(wstring re)
{
	regex = std::move(re);
	nodes = new vector<node_ptr>();
	regular_expression();
	return nodes;
}


static void set_item(shared_ptr<SetNode> &set)
{
	wchar_t c;
	while (isalnum(regex[index]) || regex[index] == '_')
	{
		c = regex[index++];
		if (match('-'))
		{
			if (c >= regex[index])
				throw runtime_error("Wrong argument in \"[a-b]\"");
			else
				set->add_set_range(make_pair(c, regex[index++]));
		} else
		{
			set->add_set_range(c);
		}
	}
}



static void _char(node_ptr &node)
{
	if (index >= regex.size())
		return;

	auto is_metachar = [](const wchar_t &c)->bool {switch (c)
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
	return false; };

	if (!is_metachar(regex[index]))
	{
		node = make_shared<CharNode>(regex[index++], false);
		nodes->push_back(node);
	}
}


static node_ptr elementary_re()
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
		\S {=not a whitespace, [^ \t\n\r\f]}
		\w {='word' character, [a-zA-Z0-9_]}
		\W {=not a 'word' character, [^a-zA-Z0-9_]}
	*/
	auto is_specialchar = [](const wchar_t &c)->bool {switch (c)
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
		if (is_specialchar(regex[index]))
			node = make_shared<CharNode>(regex[index++], true);
		else
			node = make_shared<CharNode>(regex[index++], false);
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
		node = make_shared<CharNode>('.', true);
		nodes->push_back(node);
	} else
		_char(node);

	return node;
}

static pair<int, int> range()
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
			if (num1 >= num2)
				throw runtime_error("In the operation \"{num1,num2}\",num1 must be smaller than num2.");
			else
				return make_pair(num1, num2);
		else
			throw runtime_error("Syntax error:missing '}'.");
	} else
		throw runtime_error("Syntax error:wrong symbol in the operation \"{num1,num2}\".");
}


static node_ptr basic_re()
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

static node_ptr _union()
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

static node_ptr concatenation()
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

static node_ptr simple_re()
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

static node_ptr regular_expression()
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
