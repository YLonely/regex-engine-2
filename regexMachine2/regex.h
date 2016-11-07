#pragma once
#include "regex_ast.h"

typedef std::pair<wchar_t, wchar_t> edge;
class EdgeSet
{
public:
	EdgeSet &add_edge(edge);
	std::vector<int> get_edge_index(edge);
	int get_edge_index(wchar_t);
	edge &get_edge(int index);
private:
	std::vector<edge> set;
};


class Regex
{
public:
	Regex() = default;
	Regex(std::wstring re);
	inline void set_regex(std::wstring re);

	bool match(std::wstring teststring);
	inline std::wstring &get_result() {
		return result;
	}
private:
	std::wstring regex;
	std::wstring result;
};