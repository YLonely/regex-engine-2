#include "analyzer.h"
#include "regex.h"
#include "std.h"

using namespace lw_regex;

int main() {
    try {
        Regex a(L"[0-9]+");
        Regex b(L"[a-zA-Z]+");
        Regex c(L"[^0-9a-zA-Z]+");
        std::cout << a.match(L"12345", MATCH_TYPE::ALL_MATCH) << std::endl;
        std::cout << b.match(L"abcde56abcde", MATCH_TYPE::SUB_MATCH)
                  << std::endl;
        MatchUnit num(L"num", a), ch(L"char", b), other(L"other", c);
        // MatchUnit num(L"num", L"[0-9]+"), ch(L"char", L"[a-zA-Z]+"),
        // other(L"other", L"[^0-9a-zA-Z]+");
        LexicalAnalyzer an(num, ch, other);
        an.set_target_file("./test.txt");
        token temp = an.get_next_token();
        while (!(temp.first == L"" && temp.second == L"")) {
            std::wcout << temp.first + L"  " + temp.second << std::endl;
            temp = an.get_next_token();
        }
    } catch (const engine_exception& e) {
        std::wcout << e.info() << std::endl;
    }
    getchar();
    return 0;
}
