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
	for (auto it = set.begin(); it != set.end();)
	{
		if (it->first > p.first&&it->second < p.second)
		{
			set.erase(it);
			set.push_back(make_pair(p.first, p.second));
		} else if (p.first < it->first&&p.second >= it->first)
		{
			set.push_back(make_pair(p.first, it->second));
			set.erase(it);
		} else if (it->second >= p.first&&it->second < p.second)
		{
			set.push_back(make_pair(it->first, p.second));
			set.erase(it);
		} else
			++it;
	}
}

void SetNode::add_set_range(char & ch)
{
	add_set_range(make_pair(ch, ch));
}
