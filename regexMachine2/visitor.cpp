#include "stdafx.h"
#include "visitor.h"

namespace regex_engine2_visitor {



void NFAConstructVisitor::connect(Automata &a, status_ptr &s)
{
	edge_ptr epsilon_edge = make_shared<Edge>();
	epsilon_edge->start = a.end;
	epsilon_edge->end = s;
	a.end->out_edges.push_back(epsilon_edge);
	s->in_edges.push_back(epsilon_edge);
}

void NFAConstructVisitor::connect(status_ptr &start, status_ptr &end, edge_ptr e = make_shared<Edge>())
{
	e->start = start;
	e->end = end;
	start->out_edges.push_back(e);
	end->in_edges.push_back(e);
}

void regex_engine2_visitor::NFAConstructVisitor::connect(Automata &start, Automata &end)
{
	connect(start.end, end.start);
}



Automata NFAConstructVisitor::apply(CharNode *n)
{
	status_ptr start = make_shared<Status>(status_index++);
	status_ptr end = make_shared<Status>(status_index++);
	edge_ptr char_edge = make_shared<Edge>(set.get_edge_index({ n->get_char(),n->get_char() }));
	connect(start, end, char_edge);
	return Automata(start, end);
}

Automata NFAConstructVisitor::apply(ConcatenationNode *n)
{
	Automata left = invoke(n->get_left());
	Automata right = invoke(n->get_right());
	connect(left, right);
	return Automata(left.start, right.end);
}

Automata NFAConstructVisitor::apply(RangeNode *n)
{
	return Automata();
}

Automata NFAConstructVisitor::apply(SetNode *n)
{
	status_ptr start = make_shared<Status>(status_index++);
	status_ptr end = make_shared<Status>(status_index++);
	edge_ptr char_edge = make_shared<Edge>(set.get_edge_index(n->get_set()));
	connect(start, end, char_edge);
	return Automata(start, end);
}

Automata NFAConstructVisitor::apply(AlternationNode *n)
{
	auto alter_status = make_shared<Status>(status_index++);
	Automata left = invoke(n->get_left());
	Automata right = invoke(n->get_right());
	auto end_status = make_shared<Status>(status_index++);
	connect(alter_status, left.start);
	connect(alter_status, right.start);

	connect(left.end, end_status);
	connect(right.end, end_status);

	return Automata(alter_status, end_status);
	//return Automata();
}



Automata NFAConstructVisitor::apply(StarNode *n)
{
	auto start = make_shared<Status>(status_index++);
	auto frag = invoke(n->get_node());
	auto end = make_shared<Status>(status_index++);
	connect(frag.end, frag.start);
	connect(frag.end, end);

	connect(start, frag.start);
	connect(start, end);
	return Automata(start, end);
}

Automata NFAConstructVisitor::apply(PlusNode *n)
{
	auto frag = invoke(n->get_node());
	connect(frag.end, frag.start);
	return Automata(frag.start, frag.end);
}

Automata NFAConstructVisitor::apply(QuesNode *n)
{
	auto frag = invoke(n->get_node());
	connect(frag.start, frag.end);
	return Automata(frag.start, frag.end);
}

Automata NFAConstructVisitor::apply(EndOfString *end)
{
	Automata nfa = invoke(end->get_node());
	status_ptr final = make_shared<Status>(status_index++, true);
	connect(nfa, final);
	return nfa;
}



}
