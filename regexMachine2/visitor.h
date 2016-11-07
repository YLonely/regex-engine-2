#pragma once
#include "regex_ast.h"
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
	void visit(CharNode) override;
	void visit(RangeNode) override;
	void visit(SetNode) override;
	/*void visit(ConcatenationNode) override;
	void visit(AlternationNode) override;*/
	void visit(StarNode) override;
	void visit(PlusNode) override;
	void visit(QuesNode) override;
private:
	std::vector<std::pair<wchar_t, wchar_t>> edge_set;
};