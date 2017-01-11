#include "stdafx.h"
#include "analyzer.h"

namespace regex_engine2_analyzer {
using std::vector;

token LexicalAnalyzer::get_next_token()
{
	auto &list = token_list;
	auto advance = [&list](vector<int> &statuses, wchar_t &ch)->vector<int> {
		vector<int> next_statuses;
		for (int i = 0; i < statuses.size(); ++i) {
			auto current = statuses[i];
			if (current == -1) {
				next_statuses.push_back(-1);
				continue;
			}
			Regex &re = list[i].regex;
			next_statuses.push_back(re.tran[current][re.set.get_group_index(ch)]);
		}
		return next_statuses;
	};

	auto is_end = [](vector<int> &statuses)->bool {
		for (auto i : statuses)
			if (i != -1)
				return false;
		return true;
	};

	vector<int> current_statuses(token_list.size(), 0);
	std::wstring lexeme;
	vector<int> next_statuses;
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
		next_statuses = std::move(advance(current_statuses, in_char));
		if (is_end(next_statuses))
		{
			//in_stream.seekg(-1, std::ios::cur);
			delay = true;
			break;
		}
		lexeme += in_char;
		current_statuses = std::move(next_statuses);
		//auto old_seek = std::ios::cur;
	} while (in_stream >> in_char);

	for (auto i = 0; i < current_statuses.size(); ++i)
	{
		auto current = current_statuses[i];
		if (current != -1 && token_list[i].regex.tran[current].is_final())
			return std::make_pair(token_list[i].token_name, lexeme);
	}


match_failed:
	return std::pair<std::wstring, std::wstring>();
}
}


