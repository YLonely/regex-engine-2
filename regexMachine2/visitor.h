#pragma once
#include <vector>
#include "regex_ast.h"
#include "regex.h"
#include "automata.h"


namespace regex_engine2_visitor {

using  regex_engine2_regex::EdgeSet;
using namespace regex_engine2_astnode;

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
	virtual void visit(CharNode) {}
	virtual void visit(RangeNode) {}
	virtual void visit(SetNode) {}
	virtual void visit(ConcatenationNode) {}
	virtual void visit(AlternationNode) {}
	virtual void visit(StarNode) {}
	virtual void visit(PlusNode) {}
	virtual void visit(QuesNode) {}
	virtual void visit(EndOfString) {}
};

class EdgeSetConstructVisitor :public IVisitor
{
public:
	inline void visit(CharNode n) override {
		set.add_edge({ n.get_char(),n.get_char() });
	}
	void visit(SetNode) override;
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
	void visit(CharNode) override;
	void visit(ConcatenationNode) override;
	void visit(RangeNode) override;
	void visit(SetNode) override;
	void visit(AlternationNode) override;
	void visit(StarNode) override;
	void visit(PlusNode) override;
	void visit(QuesNode) override;
	void visit(EndOfString) override;
private:
	class Fragment
	{
	public:
		Fragment() = delete;
		Fragment(status_ptr &start, std::vector<edge_ptr> out)
			:start(start), out_edges(std::move(out)) {}
		void append(Fragment &);
		inline status_ptr& get_start() {
			return start;
		}
		inline std::vector<edge_ptr>& get_out() {
			return out_edges;
		}
	private:
		status_ptr start;
		std::vector<edge_ptr> out_edges;
	};
	void connect(status_ptr &, edge_ptr &, status_ptr &);
	void attach(status_ptr &, edge_ptr &);
	std::vector<Fragment> stack;
	int state_index = 0;
	EdgeSet set;
	Automata NFA;
};

}




