#pragma once
#include "regex.h"
#include "regex_exception.h"
#include <vector>
#include <fstream>
namespace regex_engine2_analyzer {
using regex_engine2_regex::Regex;
using regex_engine2_exception::regex_runtime_error;

typedef std::pair<std::wstring, std::wstring> token;

class MatchUnit
{
	friend class LexicalAnalyzer;
public:
	MatchUnit() = default;
	MatchUnit(std::wstring name, Regex re) :token_name(std::move(name)), regex(re) {}
	inline void set(std::wstring name, Regex re) {
		token_name = name;
		regex = re;
	}
private:
	std::wstring token_name;
	Regex regex;
};


class LexicalAnalyzer
{
public:
	LexicalAnalyzer() = delete;
	template<typename T> void add_token(T& head) {
		token_list.push_back(head);
	}
	template<typename T, typename... Args> void add_token(T& head, Args&... rest) {
		token_list.push_back(head);
		add_token(rest...);
	}
	template<typename T, typename... Args> LexicalAnalyzer(T& head, Args&... rest) {
		token_list.push_back(head);
		add_token(rest...);
	}
	//This function return a pair,the first part of the pair is token's name,and the second part
	//is lexeme.when the match finish or somethine wrong with the function,the two part of the pair
	//are empty
	token get_next_token();
	inline void set_target_file(std::wstring file_path) {
		if (in_stream)
			in_stream.close();
		in_stream.open(file_path);
		if (!in_stream)
			throw regex_runtime_error(L"Open file failed.");
		in_stream.imbue(std::locale("chs"));
		in_stream >> std::noskipws;
	}
	~LexicalAnalyzer() {
		if (in_stream)
			in_stream.close();
	}
private:
	std::vector<MatchUnit> token_list;
	std::wifstream in_stream;
	bool delay = false;
	wchar_t in_char;
};


}