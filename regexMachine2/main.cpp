// regexMachine2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int main()
{
	try
	{

		regex_parse("ab[d-f_]|x?y{2,5}z*|(mn)+");
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

