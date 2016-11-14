#pragma once
#include "regex_ast.h"


namespace regex_engine2_regex {


typedef std::pair<wchar_t, wchar_t> edge;
typedef std::vector<edge>::size_type index_t;


class EdgeSet
{
public:
	EdgeSet &add_edge(edge);
	std::vector<index_t> get_edge_index(edge);
	index_t get_edge_index(wchar_t);
	edge get_edge(index_t index);
private:
	std::vector<edge> e_set;
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
	EdgeSet set;
};

}