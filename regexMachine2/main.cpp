// regexMachine2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "regex.h"

int main()
{
	//Regex a(L"[^a]");
	//Regex b(L"[^a-m]");
	try
	{
		regex_engine2_regex::EdgeSet e;
		e.add_edge({ 'a','d' });
		e.add_edge({ 'e','e' });
		e.add_edge({ 'x','z' });
		e.add_edge({ 'a','z' });
		e.add_edge({ 'g','j' });
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}

