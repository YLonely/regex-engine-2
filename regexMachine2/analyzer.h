#pragma once
#include "regex.h"
#include "regex_exception.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
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
	MatchUnit(std::wstring name, std::wstring re_exp) :token_name(std::move(name)) {
		regex.set_regex(re_exp);
	}
private:
	std::wstring token_name;
	Regex regex;
	int unit_id = -1;
};

class LexicalAnalyzer
{
public:
	LexicalAnalyzer() = delete;
	template<typename T> void add_token(T& head) {
		unit_list.push_back(head);
		for (auto i = 0; i < unit_list.size(); ++i)
			unit_list[i].unit_id = i;
	}
	template<typename T, typename... Args> void add_token(T& head, Args&... rest) {
		unit_list.push_back(head);
		add_token(rest...);
	}
	template<typename T> LexicalAnalyzer(T& head) {
		unit_list.push_back(head);
	}
	template<typename T, typename... Args> LexicalAnalyzer(T& head, Args&... rest) {
		unit_list.push_back(head);
		add_token(rest...);
	}
	//This function return a pair,the first part of the pair is token's name,and the second part
	//is lexeme.when the match finish or somethine wrong with the function,the two part of the pair
	//are empty
	token get_next_token();
	inline void set_target_file(std::wstring file_path) {
		if (pfile)
			fclose(pfile);
		pfile = _wfopen(file_path.data(), L"rb");
		if (!pfile)
			throw regex_runtime_error(L"Open file failed.");
		fseek(pfile, 2, SEEK_SET);
		buff_index = 0;
	}
	~LexicalAnalyzer() {
		if (pfile)
			fclose(pfile);
		if (current_buff)
			free(current_buff);
		if (next_buff)
			free(next_buff);
	}
private:
	wchar_t &get_next_ch();
	std::vector<MatchUnit> unit_list;
	FILE *pfile = NULL;
	bool delay = false;
	wchar_t in_char;
	wchar_t *current_buff = NULL;
	wchar_t *next_buff = NULL;
	unsigned int buff_index = 0;
};


}