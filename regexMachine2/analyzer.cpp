#include "stdafx.h"
#include "analyzer.h"
#include "automata.h"

namespace regex_engine2_analyzer {
using std::vector;
using regex_engine2_automata::dfa_index;

token LexicalAnalyzer::get_next_token()
{
	struct record
	{
		dfa_index status;
		int unit_id;
	};
	auto &list = unit_list;
	auto advance = [&list](vector<record> &records, wchar_t &ch)->vector<record> {
		vector<record> next_records;
		dfa_index next;
		for (int i = 0; i < records.size(); ++i) {
			record &r = records[i];
			dfa_index &current = r.status;
			if (current == -1)
				continue;
			Regex &re = list[r.unit_id].regex;
			next = re.tran[current][re.set.get_group_index(ch)];
			if (next == -1)
				continue;
			next_records.push_back({ next,r.unit_id });
		}
		return next_records;
	};

	/*auto is_end = [](vector<int> &statuses)->bool {
		for (auto i : statuses)
			if (i != -1)
				return false;
		return true;
	};*/

	vector<record> current_records;
	for (auto i = 0; i < list.size(); ++i)
		current_records.push_back({ 0,i });
	std::wstring lexeme;
	vector<record> next_records;
	record current_re;
	dfa_index current_status;
	if (!delay)
	{
		while (in_stream >> in_char)
		{
			if (in_char == L' ' || in_char == L'\n' || in_char == L'\t')
				continue;
			break;
		}
	} else
		delay = false;
	if (in_stream.eof())
		goto match_failed;
	do
	{
		if (in_char == L' ' || in_char == L'\n' || in_char == L'\t')
			break;
		next_records = std::move(advance(current_records, in_char));
		if (next_records.empty())
		{
			//in_stream.seekg(-1, std::ios::cur);
			delay = true;
			break;
		}
		lexeme += in_char;
		current_records = std::move(next_records);
		//auto old_seek = std::ios::cur;
	} while (in_stream >> in_char);

	current_re = current_records[0];
	current_status = current_re.status;
	if (current_status != -1 && unit_list[current_re.unit_id].regex.tran[current_status].is_final())
		return std::make_pair(unit_list[current_re.unit_id].token_name, lexeme);



match_failed:
	return std::pair<std::wstring, std::wstring>();
}
}


