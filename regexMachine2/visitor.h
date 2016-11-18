#pragma once
#include "regex_ast.h"
#include "regex.h"

namespace regex_engine2_visitor {

using  regex_engine2_regex::EdgeSet;
using namespace regex_engine2_astnode;

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
};

class EdgeSetConstructorVisitor :public IVisitor
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

}