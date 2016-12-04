#pragma once
#include "regex_ast.h"


namespace regex_engine2_regex {


typedef std::pair<wchar_t, wchar_t> edge;
typedef std::vector<edge>::size_type index_t;


/*
	The class which stores the char range of the regex,
	and devide the range into non-intersect range set.
*/
class EdgeSet
{
public:
	EdgeSet &add_edge(edge);
	std::vector<index_t> get_edge_index(edge);
	std::vector<index_t> get_edge_index(std::vector<edge>);
	index_t get_edge_index(wchar_t &);
	edge get_edge(index_t index);
	inline auto get_capacity() {
		return e_set.size();
	}
private:
	std::vector<edge> e_set;
};


class Regex
{
	friend void parse(Regex &);
public:
	Regex() = default;
	Regex(std::wstring re) {
		set_regex(std::move(re));
	}
	void set_regex(std::wstring re);

	bool match(std::wstring teststring);
	inline std::wstring &get_result() {
		return result;
	}
	std::wstring get_regex() {
		return regex;
	}
private:
	std::wstring regex;
	std::wstring result;
	EdgeSet set;
};

}