#pragma once
#include <vector>
#include "regex_ast.h"
#include "regex.h"
#include "automata.h"


namespace regex_engine2_visitor {

using  regex_engine2_regex::EdgeSet;
using namespace regex_engine2_ast;

using regex_engine2_automata::Status;
using regex_engine2_automata::Edge;
using regex_engine2_automata::Automata;
using regex_engine2_automata::edge_ptr;
using regex_engine2_automata::status_ptr;
using std::make_shared;

class IVisitor
{
public:
	virtual ~IVisitor() = default;
	virtual void visit(CharNode*) = 0;
	virtual void visit(RangeNode*) = 0;
	virtual void visit(SetNode*) = 0;
	virtual void visit(ConcatenationNode*) = 0;
	virtual void visit(AlternationNode*) = 0;
	virtual void visit(StarNode*) = 0;
	virtual void visit(PlusNode*) = 0;
	virtual void visit(QuesNode*) = 0;
	virtual void visit(EndOfString*) = 0;
};

class EdgeSetConstructVisitor :public IVisitor
{
public:
	void visit(CharNode *n) override {
		set.add_edge({ n->get_char(),n->get_char() });
	}
	void visit(SetNode *n) override {
		auto &set = n->get_set();
		for (auto &i : set)
			this->set.add_edge(i);
	}

	void visit(RangeNode*) override {}
	void visit(ConcatenationNode*) override {}
	void visit(AlternationNode*) override {}
	void visit(StarNode*) override {}
	void visit(PlusNode*) override {}
	void visit(QuesNode*) override {}
	void visit(EndOfString*) override {}


	inline EdgeSet get_set() {
		return set;
	}
private:
	EdgeSet set;
};

class NFAConstructVisitor :public IVisitor
{
public:
	NFAConstructVisitor() = delete;
	NFAConstructVisitor(EdgeSet e) :set(std::move(e)) {}

	Automata invoke(node_ptr n) {
		n->accept_visitor(this);
		return NFA;
	}

	void visit(CharNode *n) override {
		NFA = apply(n);
	}
	void visit(ConcatenationNode *n) override {
		NFA = apply(n);
	}
	void visit(RangeNode *n) override {
		NFA = apply(n);
	}
	void visit(SetNode *n) override {
		NFA = apply(n);
	}
	void visit(AlternationNode *n) override {
		NFA = apply(n);
	}
	void visit(StarNode *n) override {
		NFA = apply(n);
	}
	void visit(PlusNode *n) override {
		NFA = apply(n);
	}
	void visit(QuesNode *n) override {
		NFA = apply(n);
	}
	void visit(EndOfString *end) override {
		NFA = apply(end);
	}

	Automata apply(CharNode*);
	Automata apply(ConcatenationNode*);
	Automata apply(RangeNode*);
	Automata apply(SetNode*);
	Automata apply(AlternationNode*);
	Automata apply(StarNode*);
	Automata apply(PlusNode*);
	Automata apply(QuesNode*);
	Automata apply(EndOfString*);
private:
	void connect(Automata &, status_ptr &);
	void connect(status_ptr &, status_ptr &, edge_ptr);
	void connect(Automata &, Automata &);
	int status_index = 0;
	EdgeSet set;
	Automata NFA;
};

}




