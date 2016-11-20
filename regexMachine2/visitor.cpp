#include "stdafx.h"
#include "visitor.h"

namespace regex_engine2_visitor {

using std::make_shared;

void EdgeSetConstructVisitor::visit(SetNode n)
{
	auto &set = n.get_set();
	for (auto &i : set)
		this->set.add_edge(i);
}


void NFAConstructVisitor::visit(CharNode n)
{
	status_ptr s = make_shared<Status>();
	edge_ptr e = make_shared<Edge>();
	e->match_content = std::move(set.get_edge_index({ n.get_char(),n.get_char() }));
	e->start = s;
	s->out_edges.push_back(e);
	s->index = state_index++;
	stack.push_back(s);
	NFA.get_status().push_back(s);
	NFA.get_edges().push_back(e);
}

void NFAConstructVisitor::visit(ConcatenationNode n)
{
	auto s1 = stack.back();
	stack.pop_back();
	auto s2 = stack.back();
	stack.pop_back();

	s2->append(*s1);

	NFA.get_entry() = s2;
	status_ptr frag = make_shared<Status>(s2->in_edges, s1->out_edges);

	stack.push_back(frag);
}

}
