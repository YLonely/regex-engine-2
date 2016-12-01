#pragma once
#include <utility>
#include <vector>
#include <memory>

namespace regex_engine2_visitor {

class IVisitor;

}

namespace regex_engine2_ast {

using regex_engine2_visitor::IVisitor;

class IASTNode
{
public:
	virtual void accept_visitor(IVisitor &visitor) = 0;
	virtual ~IASTNode() = default;
};

typedef std::shared_ptr<IASTNode> node_ptr;


class AST
{
public:
	AST() = default;
	AST(std::vector<node_ptr> *nodes) :nodes(nodes) {}
	inline std::vector<node_ptr> *&get_nodes() {
		return nodes;
	}
	inline node_ptr get_root() {
		return nodes->back();
	}
	void release_nodes();
private:
	std::vector<node_ptr> *nodes;
};


class CharNode :public IASTNode
{
public:
	CharNode() = delete;
	CharNode(wchar_t ch) :c(ch) {}
	void accept_visitor(IVisitor &visitor) override;
	inline wchar_t get_char() {
		return c;
	}
private:
	wchar_t c = -1;
};

class RangeNode :public IASTNode
{
public:
	RangeNode() = delete;
	RangeNode(node_ptr node, int min, int max) :node(node), min(min), max(max) {}
	void accept_visitor(IVisitor &visitor) override;
	std::pair<int, int> get_range() {
		return{ min,max };
	}
private:
	node_ptr node;
	int min;
	int max;	//max = -1 means infinity
};

class SetNode :public IASTNode
{
public:
	SetNode() = delete;
	SetNode(bool ispositive) :ispositive(ispositive) {}
	SetNode &add_set_range(const wchar_t &, const wchar_t &);
	SetNode &add_set_range(const wchar_t &);
	inline std::vector<std::pair<wchar_t, wchar_t>> &get_set() {
		return set;
	}
	void merge();
	void accept_visitor(IVisitor &visitor) override;
private:
	std::vector<std::pair<wchar_t, wchar_t>> set;
	bool ispositive = true;
};

class ConcatenationNode :public IASTNode
{
public:
	ConcatenationNode() = delete;
	ConcatenationNode(node_ptr left, node_ptr right) :left(left), right(right) {}
	void accept_visitor(IVisitor &visitor) override;
private:
	node_ptr left;
	node_ptr right;
};

class AlternationNode :public IASTNode
{
public:
	AlternationNode() = delete;
	AlternationNode(node_ptr left, node_ptr right) :left(left), right(right) {}
	void accept_visitor(IVisitor &visitor) override;
private:
	node_ptr left;
	node_ptr right;
};

class StarNode :public IASTNode
{
public:
	StarNode() = delete;
	StarNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor &visitor) override;
private:
	node_ptr node;
};

class PlusNode :public IASTNode
{
public:
	PlusNode() = delete;
	PlusNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor &visitor) override;
private:
	node_ptr node;
};

class QuesNode :public IASTNode
{
public:
	QuesNode() = delete;
	QuesNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor &visitor) override;
private:
	node_ptr node;
};

class EndOfString :public IASTNode
{
public:
	EndOfString() = default;
	EndOfString(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor &visitor) override;
private:
	node_ptr node;
};

}

