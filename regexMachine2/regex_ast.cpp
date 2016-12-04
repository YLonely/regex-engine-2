#include "stdafx.h"
#include "regex_ast.h"
#include "visitor.h"

namespace regex_engine2_ast {

using std::pair;
using std::vector;
using std::make_pair;

void CharNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

void RangeNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

SetNode &SetNode::add_set_range(const wchar_t &ch1, const wchar_t &ch2)
{
	set.push_back({ ch1, ch2 });
	return *this;
}

SetNode &SetNode::add_set_range(const wchar_t &ch)
{
	set.push_back({ ch, ch });
	return *this;
}


/*
	This function will
	1.merge the duplicate characters in the SetNode.	i.e [aba-d] will be transfered into [a-d].
	2.reverse the negetive set to positive character range.
*/
void SetNode::merge()
{
	if (set.size() != 1)
	{
		sort(set.begin(), set.end(), [](const pair<wchar_t, wchar_t> &p1, const pair<wchar_t, wchar_t> &p2) {
			return p1.first < p2.first;
		});
		int count = 1;
		wchar_t temp1, temp2;
		for (auto it1 = set.begin(), it2 = set.begin() + 1;;)
		{
			if (it2 != set.end() && (it2 - 1)->second + 1 >= it2->first)
			{
				++it2;
				++count;
			} else
			{
				if (count > 1)
				{
					temp1 = it1->first;
					temp2 = it1->second > (it2 - 1)->second ? it1->second : (it2 - 1)->second;
					it1 = set.erase(it1, it2);
					it1 = set.insert(it1, { temp1, temp2 });
					it2 = it1 + 1;
					count = 1;
				} else
				{
					if (it2 == set.end())
						break;
					it1 = it2;
					++it2;
				}
			}
		}
	}
	vector<pair<wchar_t, wchar_t>> set3;
	if (!ispositive)
	{
		ispositive = true;
		if (set.size() == 1)
		{
			if (set[0].first > (wchar_t)0)
				set3.push_back({ (wchar_t)0, set[0].first - 1 });
			if (set[0].second < (wchar_t)65535)
				set3.push_back({ set[0].second + 1, (wchar_t)65535 });
		} else
		{
			auto it1 = set.begin(), it2 = set.begin() + 1;
			for (; it2 != set.end(); ++it1, ++it2)
			{
				if (it1 == set.begin() && it1->first >(wchar_t)0)
					set3.push_back({ (wchar_t)0, it1->first - 1 });
				set3.push_back({ it1->second + 1, it2->first - 1 });
			}
			if (it1->second < (wchar_t)65535)
				set3.push_back({ it1->second + 1, (wchar_t)65535 });
		}
		set = std::move(set3);
	}
}

void SetNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

void ConcatenationNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

void AlternationNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

void StarNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

void PlusNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

void QuesNode::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}


void EndOfString::accept_visitor(IVisitor *visitor)
{
	visitor->visit(this);
}

}