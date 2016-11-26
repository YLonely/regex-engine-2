#include "stdafx.h"
#include "visitor.h"

namespace regex_engine2_visitor {




void EdgeSetConstructVisitor::visit(SetNode n)
{
	auto &set = n.get_set();
	for (auto &i : set)
		this->set.add_edge(i);
}

void NFAConstructVisitor::connect(status_ptr &start, edge_ptr &edge, status_ptr &end)
{
	bool contain = false;
	edge->start = start;
	edge->end = end;
	for (auto &e : start->out_edges)
		if (e == edge)
		{
			contain = true;
			break;
		}
	if (!contain)
		start->out_edges.push_back(edge);

	contain = false;

	for (auto &e : end->in_edges)
		if (e == edge)
		{
			contain = true;
			break;
		}
	if (!contain)
		end->in_edges.push_back(edge);
}

void NFAConstructVisitor::attach(status_ptr &start, edge_ptr &edge)
{
	bool contain = false;
	edge->start = start;
	for (auto &e : start->out_edges)
		if (e == edge)
		{
			contain = true;
			break;
		}
	if (!contain)
		start->out_edges.push_back(edge);
}

void NFAConstructVisitor::visit(CharNode n)
{
	status_ptr s = make_shared<Status>(status_index++);
	edge_ptr e = make_shared<Edge>(std::move(set.get_edge_index({ n.get_char(),n.get_char() })));
	attach(s, e);
	stack.push_back(Fragment(s, s->out_edges));
	NFA.get_status().push_back(s);
	NFA.get_edges().push_back(e);
}

void NFAConstructVisitor::visit(ConcatenationNode)
{
	auto f1 = stack.back();
	stack.pop_back();
	auto f2 = stack.back();
	stack.pop_back();

	f2.append(f1);

	NFA.get_entry() = f2.get_start();

	stack.push_back(Fragment(f2.get_start(), f1.get_out()));
}

void NFAConstructVisitor::visit(RangeNode n)
{
	auto f = stack.back();
	stack.pop_back();

	int min = n.get_range().first, max = n.get_range().second;
	auto e_edge4 = make_shared<Edge>();
	auto first_s = make_shared<Status>(status_index++);
	if (!min)
	{
		
	} else
	{
		auto r_edge=make_shared<Edge>(true,)
		attach(first_s, e_edge4);
		for (auto &e : f.get_out())
		{
			e->end = first_s;
			first_s->in_edges.push_back(e);
		}
		connect(first_s,)
	}

	NFA.get_edges().push_back(e_edge4);
	NFA.get_status().push_back(first_s);
	NFA.get_entry() = f.get_start();
}

void NFAConstructVisitor::visit(SetNode n)
{
	status_ptr s = make_shared<Status>(status_index++);
	edge_ptr e = make_shared<Edge>(std::move(set.get_edge_index(n.get_set())));
	attach(s, e);
	stack.push_back(Fragment(s, s->out_edges));
	NFA.get_status().push_back(s);
	NFA.get_edges().push_back(e);
}

void NFAConstructVisitor::visit(AlternationNode)
{
	auto f1 = stack.back();
	stack.pop_back();
	auto f2 = stack.back();
	stack.pop_back();

	std::vector<edge_ptr> merge;
	for (auto &e : f1.get_out())
		merge.push_back(e);
	for (auto &e : f2.get_out())
		merge.push_back(e);

	auto alter_state = make_shared<Status>(status_index++);
	auto edge1 = make_shared<Edge>();
	auto edge2 = make_shared<Edge>();

	connect(alter_state, edge1, f1.get_start());
	connect(alter_state, edge2, f2.get_start());

	NFA.get_edges().push_back(edge1);
	NFA.get_edges().push_back(edge2);
	NFA.get_status().push_back(alter_state);
	NFA.get_entry() = alter_state;
	stack.push_back(Fragment(alter_state, merge));
}

void NFAConstructVisitor::visit(StarNode)
{
	auto f = stack.back();
	stack.pop_back();

	auto first_s = make_shared<Status>(), end_s = make_shared<Status>();
	auto e_edge1 = make_shared<Edge>(), e_edge2 = make_shared<Edge>(),
		e_edge3 = make_shared<Edge>(), r_edge = make_shared<Edge>(true, 1, -1);

	attach(end_s, e_edge3);
	connect(first_s, e_edge2, end_s);
	connect(first_s, r_edge, f.get_start());
	connect(f.get_start(), e_edge1, end_s);

	for (auto &e : f.get_out())
	{
		e->end = first_s;
		first_s->in_edges.push_back(e);
	}

	stack.push_back(Fragment(f.get_start(), end_s->out_edges));

	NFA.get_entry() = f.get_start();

	NFA.get_edges().push_back(e_edge1);
	NFA.get_edges().push_back(e_edge2);
	NFA.get_edges().push_back(e_edge3);
	NFA.get_edges().push_back(r_edge);

	NFA.get_status().push_back(first_s);
	NFA.get_status().push_back(end_s);


}

void NFAConstructVisitor::visit(PlusNode)
{
	auto f = stack.back();
	stack.pop_back();
	auto e_edge1 = make_shared<Edge>(true, 1, -1);
	auto e_edge2 = make_shared<Edge>();
	auto s = make_shared<Status>(status_index++);

	attach(s, e_edge2);
	connect(s, e_edge1, f.get_start());

	for (auto &e : f.get_out())
	{
		e->end = s;
		s->in_edges.push_back(e);
	}

	stack.push_back(Fragment(f.get_start(), s->out_edges));

	NFA.get_edges().push_back(e_edge1);
	NFA.get_edges().push_back(e_edge2);
	NFA.get_status().push_back(s);
}

void NFAConstructVisitor::visit(QuesNode)
{
	auto f = stack.back();
	stack.pop_back();
	auto e_edge1 = make_shared<Edge>();
	auto e_edge2 = make_shared<Edge>();
	auto s = make_shared<Status>(status_index++);

	attach(s, e_edge2);
	connect(f.get_start(), e_edge1, s);

	for (auto &e : f.get_out())
	{
		e->end = s;
		s->in_edges.push_back(e);
	}

	stack.push_back(Fragment(f.get_start(), s->out_edges));

	NFA.get_edges().push_back(e_edge1);
	NFA.get_edges().push_back(e_edge2);
	NFA.get_status().push_back(s);
}

void NFAConstructVisitor::visit(EndOfString)
{
	auto end_state = make_shared<Status>(status_index++);
	end_state->final_status = true;
	auto f = stack.back();

	for (auto &e : f.get_out())
	{
		e->end = end_state;
		end_state->in_edges.push_back(e);
	}

	NFA.get_status().push_back(end_state);
}



void NFAConstructVisitor::Fragment::append(Fragment &f)
{
	for (auto &e : this->out_edges)
	{
		e->end = f.start;
		f.start->in_edges.push_back(e);
	}
}

}
