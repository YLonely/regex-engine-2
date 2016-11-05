#include "stdafx.h"



void CharNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void RangeNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void SetNode::add_set_range(pair<wchar_t, wchar_t> &p)
{
	set.push_back(p);
}

void SetNode::add_set_range(wchar_t & ch)
{
	set.push_back(make_pair(ch, ch));
}

void SetNode::merge()
{
	if (set.size() == 1)
		return;
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
				it1 = set.insert(it1, make_pair(temp1, temp2));
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

void SetNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void ConcatenationNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void AlternationNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void StarNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void PlusNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}

void QuesNode::accept_visitor(IVisitor visitor)
{
	visitor.visit(*this);
}
