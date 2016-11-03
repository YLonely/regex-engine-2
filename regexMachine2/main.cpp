// regexMachine2.cpp : 定义控制台应用程序的入口点。
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

