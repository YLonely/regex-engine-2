#include "stdafx.h"
#include "automata.h"


namespace regex_engine2_automata {
using std::make_shared;

void Status::append(Status &s)
{
	for (auto &e : this->out_edges)
	{
		e->end = make_shared<Status>(s);
		s.in_edges.push_back(e);
	}

}

}

