#pragma once
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include "regex.h"
#include "regex_exception.h"
namespace lw_regex {

typedef std::pair<std::wstring, std::wstring> token;

class MatchUnit {
    friend class LexicalAnalyzer;

   public:
    MatchUnit() = default;
    MatchUnit(std::wstring name, Regex re)
        : token_name(std::move(name)), regex(re) {}
    inline void set(std::wstring name, Regex re) {
        token_name = name;
        regex = re;
    }
    MatchUnit(std::wstring name, std::wstring re_exp)
        : token_name(std::move(name)) {
        regex.set_regex(re_exp);
    }

   private:
    std::wstring token_name;
    Regex regex;
    int unit_id = -1;
};

class LexicalAnalyzer {
   public:
    LexicalAnalyzer() = delete;
    template <typename T>
    void add_token(T& head) {
        unit_list.push_back(head);
        for (auto i = 0; i < unit_list.size(); ++i)
            unit_list[i].unit_id = i;
    }
    template <typename T, typename... Args>
    void add_token(T& head, Args&... rest) {
        unit_list.push_back(head);
        add_token(rest...);
    }
    template <typename T>
    LexicalAnalyzer(T& head) {
        unit_list.push_back(head);
    }
    template <typename T, typename... Args>
    LexicalAnalyzer(T& head, Args&... rest) {
        unit_list.push_back(head);
        add_token(rest...);
    }
    // This function return a pair,the first part of the pair is token's
    // name,and the second part is lexeme.when the match finish or somethine
    // wrong with the function,the two part of the pair are empty
    token get_next_token();
    inline void set_target_file(std::string file_path) {
        std::wifstream wif(file_path);
        if (!wif.is_open())
            throw regex_runtime_error(L"Open file failed.");
        wif.imbue(std::locale("zh_CN.UTF-8"));
        std::wstringstream wss;
        wss << wif.rdbuf();
        this->content = wss.str();
    }

   private:
    wchar_t& get_next_ch();
    std::vector<MatchUnit> unit_list;
    bool delay = false;
    wchar_t in_char;
    unsigned int buff_index = 0;
    std::wstring content;
};

}  // namespace lw_regex