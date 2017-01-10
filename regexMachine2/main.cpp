// regexMachine2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "regex.h"
using regex_engine2_regex::Regex;
using regex_engine2_regex::MATCH_TYPE;
int main()
{
	//Regex a(L"[^a]");
	//Regex b(L"[^a-m]");
	try
	{
		Regex a(L"(abcd|(abb)*ab){2,79}");
		//Regex b(L"(a|b)*abb{23,45}");
		//std::cout << "123\n \030" << std::endl;
		if (a.match(L"abcdabbab", MATCH_TYPE::SUB_MATCH))
			std::cout << "true" << std::endl;

		/*wchar_t m = L'你';
		std::wcout.imbue(std::locale("chs"));
		std::wcout << m << std::endl;*/
	}
	catch (const engine_exception& e)
	{
		//std::cout << e.what() << std::endl;
		e.info();
	}
	system("pause");
	return 0;
}

