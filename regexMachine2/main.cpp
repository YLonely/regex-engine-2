// regexMachine2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int main()
{
	//Regex a(L"[^a]");
	//Regex b(L"[^a-m]");
	try
	{
		Regex c(L"[\\S-d]");

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}

