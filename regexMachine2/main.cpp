// regexMachine2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "regex.h"

using regex_engine2_regex::Regex;

int main()
{
	//Regex a(L"[^a]");
	//Regex b(L"[^a-m]");

	
	try
	{
		Regex a(L"ab[d-g]{2,5}cq?e*");
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}

