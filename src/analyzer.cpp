#include "analyzer.h"
#include "automata.h"

namespace lw_regex {

using internal_automata::dfa_index;
using std::vector;
#define BUFF_SIZE 5000

token LexicalAnalyzer::get_next_token() {
    struct record {
        dfa_index status;
        int unit_id;
    };
    auto& list = unit_list;
    auto advance = [&list](vector<record>*& records, wchar_t& ch,
                           vector<record>*& next_records) {
        next_records->clear();
        // dfa_index next;
        for (int i = 0; i < records->size(); ++i) {
            record& r = (*records)[i];
            dfa_index& current = r.status;
            if (current == -1)
                continue;
            Regex& re = list[r.unit_id].regex;
            dfa_index& next = re.tran[current][re.set.get_group_index(ch)];
            if (next == -1)
                continue;
            next_records->push_back({next, r.unit_id});
        }
    };

    vector<record>* current_records = new vector<record>();
    for (auto i = 0; i < list.size(); ++i)
        current_records->push_back({0, i});
    std::wstring lexeme;
    vector<record>* next_records = new vector<record>();
    record current_re;
    dfa_index current_status;
    if (!delay) {
        while (in_char = get_next_ch()) {
            if (in_char == L' ' || in_char == L'\n' || in_char == L'\t')
                continue;
            break;
        }
    } else
        delay = false;
    if (!in_char)
        goto match_failed;
    do {
        if (in_char == L' ' || in_char == L'\n' || in_char == L'\t')
            break;
        advance(current_records, in_char, next_records);
        if (next_records->empty()) {
            // in_stream.seekg(-1, std::ios::cur);
            delay = true;
            break;
        }
        lexeme += in_char;
        auto temp = current_records;
        current_records = next_records;
        next_records = temp;
        // auto old_seek = std::ios::cur;
    } while (in_char = get_next_ch());

    current_re = (*current_records)[0];
    current_status = current_re.status;
    if (current_status != -1 &&
        unit_list[current_re.unit_id].regex.tran[current_status].is_final())
        return std::make_pair(unit_list[current_re.unit_id].token_name, lexeme);

match_failed:
    return std::pair<std::wstring, std::wstring>();
}

wchar_t& LexicalAnalyzer::get_next_ch() {
    auto res = this->content[this->buff_index++];
    return res;
}
#undef BUFF_SIZE
}  // namespace lw_regex
