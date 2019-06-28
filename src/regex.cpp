#include "regex.h"
#include <tuple>
#include "automata.h"
#include "regex_exception.h"
#include "std.h"
#include "visitor.h"

namespace lw_regex {
namespace internal_automata_parse {

std::tuple<lw_regex::internal_charset::CharSet,
           lw_regex::internal_automata::Dtran>
automata_parse(std::wstring restring);
}
}  // namespace lw_regex

namespace lw_regex {

using std::vector;
using std::wstring;

using lw_regex::regex_runtime_error;
using lw_regex::internal_automata::dfa_index;
using lw_regex::internal_automata_parse::automata_parse;
using lw_regex::internal_charset::CharSet;
using std::make_pair;
using std::pair;
using std::tuple;

int regex_match(wstring& test, unsigned int ppos, Regex& re);
void parse(Regex& re) {
    auto result = automata_parse(re.regex);
    re.set = std::move(std::get<0>(result));
    re.tran = std::move(std::get<1>(result));
}

void Regex::set_regex(wstring re) {
    if (regex != re) {
        result.clear();
        regex = std::move(re);
        parse(*this);
    }
}

/*  
    Retures wether the teststring matches the Regex.
    There are two types of MATCH_TYPE:
        - MATCH_TYPE::ALL_MATCH  return true if and only if the whole teststring matches the regex.
        - MATCH_TYPE::SUB_MATCH  return true if part of the teststring matches the regex.
*/
bool Regex::match(wstring teststring, MATCH_TYPE type) {
    int matched_pos;
    if (type == ALL_MATCH) {
        matched_pos = regex_match(teststring, 0, *this);
        if (matched_pos == teststring.length())
            return true;
        else
            return false;
    }

    for (unsigned int i = 0; i < teststring.length(); ++i) {
        matched_pos = regex_match(teststring, i, *this);
        if (matched_pos == -1)
            continue;
        result.push_back({i, wstring(teststring, i, matched_pos - i)});
        i = matched_pos - 1;
    }

    if (result.empty())
        return false;
    return true;
}

int regex_match(wstring& test, unsigned int begin_pos, Regex& re) {
    auto pos = begin_pos;
    int matched_pos = -1;
    assert(re.tran.size() >= 1);
    assert(pos <= test.length());
    dfa_index current_status = (dfa_index)0;
    if (re.tran[current_status].is_final() && begin_pos == 0)
        matched_pos = pos;
    if (!test.empty()) {
        dfa_index next_status =
            re.tran[current_status][re.set.get_group_index(test[pos++])];
        while (next_status != -1 && pos < test.length()) {
            if (re.tran[next_status].is_final())
                matched_pos = pos;
            current_status = next_status;
            next_status =
                re.tran[current_status][re.set.get_group_index(test[pos++])];
        }
        if (next_status != -1 && re.tran[next_status].is_final())
            matched_pos = pos;
    }
    return matched_pos;
}

CharSet& CharSet::add_group(char_group e) {
    if (e_set.empty())
        e_set.push_back(std::move(e));
    else {
        vector<char_group> temp;
        for (auto it = e_set.begin(); it != e_set.end(); ++it) {
            if (e.second < it->first) {
                temp.push_back(std::move(e));
                temp.insert(temp.end(), it, e_set.end());
                break;
            } else if (e.first <= it->first && e.second >= it->first &&
                       e.second <= it->second) {
                if (e.first == it->first && e.second == it->second) {
                    temp.insert(temp.end(), it, e_set.end());
                    break;
                } else if (e.first == it->first) {
                    temp.push_back({e.first, e.second});
                    temp.push_back({e.second + 1, it->second});
                } else if (e.second == it->second) {
                    temp.push_back({e.first, it->first - 1});
                    temp.push_back({it->first, it->second});
                } else {
                    temp.push_back({e.first, it->first - 1});
                    temp.push_back({it->first, e.second});
                    temp.push_back({e.second + 1, it->second});
                }
                temp.insert(temp.end(), it + 1, e_set.end());
                break;
            } else if (e.first <= it->first && e.second > it->second) {
                if (e.first == it->first) {
                    temp.push_back({it->first, it->second});
                } else {
                    temp.push_back({e.first, it->first - 1});
                    temp.push_back({it->first, it->second});
                }
                e.first = it->second + 1;
            } else if (e.second <= it->second && e.first > it->first) {
                if (e.second == it->second) {
                    temp.push_back({it->first, e.first - 1});
                    temp.push_back({e.first, e.second});
                } else {
                    temp.push_back({it->first, e.first - 1});
                    temp.push_back({e.first, e.second});
                    temp.push_back({e.second + 1, it->second});
                }
                temp.insert(temp.end(), it + 1, e_set.end());
                break;
            } else if (e.second > it->second && e.first >= it->first &&
                       e.first <= it->second) {
                if (e.first == it->first) {
                    temp.push_back({it->first, it->second});
                } else if (e.first == it->second) {
                    temp.push_back({it->first, e.first - 1});
                    temp.push_back({e.first, e.first});
                } else {
                    temp.push_back({it->first, e.first - 1});
                    temp.push_back({e.first, it->second});
                }
                e.first = it->second + 1;
            } else
                temp.push_back(*it);
            if (it + 1 == e_set.end()) {
                temp.push_back(std::move(e));
                break;
            }
        }
        e_set = std::move(temp);
    }
    return *this;
}

std::vector<group_index> CharSet::get_group_index(char_group e) {
    vector<group_index> temp;
    bool state = false;
    for (group_index i = 0; i < e_set.size(); ++i) {
        if (!state && e.first == e_set[i].first) {
            state = true;
            temp.push_back(i);
            if (e.second == e_set[i].second) {
                state = false;
                break;
            }
        } else if (state && e.second == e_set[i].second) {
            state = false;
            temp.push_back(i);
        } else if (state)
            temp.push_back(i);
    }
    if (state)
        return {};
    return temp;
}

/*
        edges must be non-intersect and only be used for SetNode.
*/
std::vector<group_index> CharSet::get_group_index(
    std::vector<char_group> edges) {
    vector<group_index> result;
    vector<group_index> temp;
    for (auto& e : edges) {
        temp = std::move(get_group_index(e));
        for (auto& i : temp)
            result.push_back(i);
    }
    return result;
}

group_index CharSet::get_group_index(wchar_t& ch) {
    group_index index = e_set.size();
    char_group temp;
    if (e_set.empty())
        return index;
    for (group_index i = 0; i < e_set.size(); ++i) {
        temp = e_set[i];
        if (ch >= temp.first && ch <= temp.second) {
            index = i;
            break;
        }
    }
    return index;
}

char_group CharSet::get_group(group_index index) {
    if (index >= e_set.size())
        throw regex_runtime_error(L"Get edge out of range.");
    return e_set[index];
}

}  // namespace lw_regex