// regexMachine2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int main()
{
	try
	{

		regex_parse("((abc[d-f]|g)))");
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	system("pause");
	return 0;
}

