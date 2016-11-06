#pragma once
#include "regex_ast.h"

class Regex
{
private:
	class EdgeSet
	{
		
	};

public:
	Regex() = default;
	Regex(wstring re);
	inline void set_regex(wstring re);

	bool match(wstring teststring);
	inline wstring &get_result() {
		return result;
	}
private:
	wstring regex;
	wstring result;
};