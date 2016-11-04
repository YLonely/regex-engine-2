#include "stdafx.h"



void CharNode::accept_visitor(IVisitor visitor)
{
}

void CharNode::operation()
{
}

void RangeNode::accept_visitor(IVisitor visitor)
{
}

void RangeNode::operation()
{
}

void SetNode::add_set_range(pair<char, char> &p)
{
	set.push_back(p);
}

void SetNode::add_set_range(char & ch)
{
	set.push_back(make_pair(ch, ch));
}

void SetNode::merge()
{
	if (set.size() == 1)
		return;
	sort(set.begin(), set.end(), [](const pair<char, char> &p1, const pair<char, char> &p2) {
		return p1.first < p2.first;
	});
	int count = 1;
	char temp1, temp2;
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
}

void SetNode::operation()
{

}

void ConcatenationNode::accept_visitor(IVisitor visitor)
{
}

void ConcatenationNode::operation()
{
}

void AlternationNode::accept_visitor(IVisitor visitor)
{
}

void AlternationNode::operation()
{
}

void StarNode::accept_visitor(IVisitor visitor)
{
}

void StarNode::operation()
{
}

void PlusNode::accept_visitor(IVisitor visitor)
{
}

void PlusNode::operation()
{
}

void QuesNode::accept_visitor(IVisitor visitor)
{
}

void QuesNode::operation()
{
}
