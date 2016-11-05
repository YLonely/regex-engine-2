#pragma once
#include "regex_ast.h"

class Regex
{
public:
	Regex() = default;
	Regex(wstring re) :regex(std::move(re)) {}
	inline void set_regex(wstring re) {
		regex = std::move(re);
	}

	bool match(wstring teststring);
	inline wstring &get_result() {
		return result;
	}
private:
	wstring regex;
	wstring result;
};