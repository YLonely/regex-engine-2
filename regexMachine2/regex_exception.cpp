#include "stdafx.h"
#include "regex_exception.h"
#include <windows.h>


namespace regex_engine2_exception {
using std::wstring;

void info_handler(const wstring pre_detial, const wstring &detial, const wstring &regex, const wstring::size_type &pos)
{
	std::wcout.imbue(std::locale("chs"));

	wstring val = pre_detial + detial + L";" ;
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

std::wstring s2ws(const std::string& s)
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

void arguement_error::info() const
{
	info_handler(L"Arguement error:", detial, regex, pos);
}

void syntax_error::info() const
{
	info_handler(L"Syntax error:", detial, regex, pos);
}

void regex_runtime_error::info() const
{
	std::wcout << detial << std::endl;
}

}
