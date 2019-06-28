#include "regex_exception.h"
#include "std.h"

namespace lw_regex {
using std::wstring;

wstring info_handler(const wstring pre_detail,
                     const wstring& detail,
                     const wstring& regex,
                     const wstring::size_type& pos) {
    wstring tmp = pre_detail + detail + L";";
    wstring regex_string = L"Regex:" + regex;
    wstring pos_string = L"";
    for (wstring::size_type i = 0; i <= pos; i++) {
        if (i == pos)
            pos_string += L"^";
        else
            pos_string += L" ";
    }
    return regex_string + L"\n" + pos_string + L"\n" + tmp + L"\n";
}

std::wstring s2ws(const std::string& s) {
    setlocale(LC_ALL, "chs");
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t* _Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest, _Source, _Dsize);
    std::wstring result = _Dest;
    delete[] _Dest;
    setlocale(LC_ALL, "C");
    return result;
}

std::wstring arguement_error::info() const {
    return info_handler(L"Arguement error:", detail, regex, pos);
}

std::wstring syntax_error::info() const {
    return info_handler(L"Syntax error:", detail, regex, pos);
}

std::wstring regex_runtime_error::info() const {
    return detail;
}

}  // namespace lw_regex
