#pragma once

namespace regex_engine2_regex {


typedef std::pair<wchar_t, wchar_t> char_group;
typedef std::vector<char_group>::size_type group_index;


/*
	The class which stores the char range of the regex,
	and devide the range into non-intersect range set.
*/
class CharSet
{
public:
	CharSet &add_group(char_group);
	std::vector<group_index> get_group_index(char_group);
	std::vector<group_index> get_group_index(std::vector<char_group>);
	group_index get_group_index(wchar_t &);
	char_group get_group(group_index index);
	inline auto get_max_index() {
		return (group_index)e_set.size();
	}
private:
	std::vector<char_group> e_set;
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
	CharSet set;
};

}