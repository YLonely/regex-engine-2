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
		//Regex a(L"(a|b)*abb");
		//Regex b(L"(a|b)*abb{23,45}");
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}

