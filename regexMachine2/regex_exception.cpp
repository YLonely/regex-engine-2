#include "stdafx.h"
#include "regex_exception.h"
#include <windows.h>


namespace regex_engine2_exception {
using std::wstring;

void arguement_error::info() const
{
	std::wcout.imbue(std::locale("chs"));

	wstring val = L"Arguement error:" + detial + L";" ;
	std::wcout << L"Regex:";
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	wstring::size_type i = 0;
	for (auto it = regex.begin(); it != regex.end(); ++it, ++i)
	{
		if (i == pos - 1)
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo(handle, &info);
			SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY);
			std::wcout << *it;
			SetConsoleTextAttribute(handle, info.wAttributes);
		} else
			std::wcout << *it;
	}
	std::cout << '\n';
	std::wcout << val;
	std::cout << '\n';
}


std::wstring arguement_error::s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[] _Dest;
	setlocale(LC_ALL, "C");
	return result;
}

}
