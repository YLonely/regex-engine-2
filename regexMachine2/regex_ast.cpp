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
