// regexMachine2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "regex.h"

int main()
{
	//Regex a(L"[^a]");
	//Regex b(L"[^a-m]");
	try
	{
		regex_engine2_regex::Regex c(L"[\\S-d]");

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}

