// regexMachine2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "regex.h"
#include "analyzer.h"
#include <fstream>

using regex_engine2_regex::Regex;
using regex_engine2_regex::MATCH_TYPE;
using regex_engine2_analyzer::LexicalAnalyzer;
using regex_engine2_analyzer::MatchUnit;
using regex_engine2_analyzer::token;
int main()
{
	long long i = 0;
	try
	{
		Regex a(L"[0-9]+");
		Regex b(L"[a-zA-z]+");
		Regex c(L"[^0-9a-zA-Z]+");
		MatchUnit num(L"num", a), ch(L"char", b), other(L"other", c);
		LexicalAnalyzer an(num, ch, other);
		an.set_target_file(L"D:\\Personal\\Desktop\\test.txt");
		token temp = an.get_next_token();
		while (!temp.first.empty())
		{
			temp = an.get_next_token();
		}
		
	}
	catch (const engine_exception& e)
	{
		//std::cout << e.what() << std::endl;
		e.info();
	}
	std::cout << i << std::endl;
	system("pause");
	return 0;
}

