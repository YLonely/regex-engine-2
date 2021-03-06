#pragma once
#include <vector>
#include "automata.h"
#include "regex_exception.h"

namespace lw_regex {
class LexicalAnalyzer;
}

namespace lw_regex {

enum MATCH_TYPE { SUB_MATCH, ALL_MATCH };

using internal_automata::char_group;
using internal_automata::Dtran;
using internal_automata::group_index;

using internal_automata::Dtran;

namespace internal_charset {

class CharSet {
   public:
    CharSet& add_group(char_group);
    std::vector<group_index> get_group_index(char_group);
    std::vector<group_index> get_group_index(std::vector<char_group>);
    group_index get_group_index(wchar_t&);
    char_group get_group(group_index index);
    inline group_index size() { return (group_index)e_set.size(); }

   private:
    std::vector<char_group> e_set;
};

}  // namespace internal_charset
/*
        The class which stores the char range of the regex,
        and devide the range into non-intersect range set.
*/

class Regex {
    friend void parse(Regex&);
    friend int regex_match(std::wstring&, unsigned int, Regex&);
    friend class LexicalAnalyzer;

   public:
    Regex() = default;
    Regex(std::wstring re) { set_regex(std::move(re)); }
    void set_regex(std::wstring re);

    bool match(std::wstring teststring, MATCH_TYPE);

    /*
        Return a list of match result, the first part of pair indicates the begin index of matched substring, the second part is the matched string.
     */
    inline std::vector<std::pair<unsigned int, std::wstring>>& get_result() {
        return result;
    }
    std::wstring get_regex() { return regex; }

   private:
    std::wstring regex;
    // Record the match result of SUB_MATCH
    std::vector<std::pair<unsigned int, std::wstring>> result;
    internal_charset::CharSet set;
    Dtran tran;
};

}  // namespace lw_regex