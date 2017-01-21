// regexMachine2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "regex.h"
#include "analyzer.h"


using regex_engine2_regex::Regex;
using regex_engine2_regex::MATCH_TYPE;
using regex_engine2_analyzer::LexicalAnalyzer;
using regex_engine2_analyzer::MatchUnit;
using regex_engine2_analyzer::token;
int main()
{
	try
	{
		Regex a(L"[0-9]+");
		Regex b(L"[a-zA-Z]+");
		Regex c(L"[^0-9a-zA-Z]+");
		a.match(L"12345", MATCH_TYPE::ALL_MATCH);
		b.match(L"abcde56abcde", MATCH_TYPE::SUB_MATCH);
		MatchUnit num(L"num", a), ch(L"char", b), other(L"other", c);
		//亦可MatchUnit num(L"num", L"[0-9]+"), ch(L"char", L"[a-zA-Z]+"), other(L"other", L"[^0-9a-zA-Z]+");
		LexicalAnalyzer an(num, ch, other);
		an.set_target_file(L"test.txt");
		token temp = an.get_next_token();
	}
	catch (const engine_exception& e)
	{
		e.info();
	}
	system("pause");
	return 0;
}

