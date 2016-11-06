#pragma once
#include <utility>
#include <vector>
#include <memory>
using namespace std;
class IVisitor;
class IASTNode
{
public:
	virtual void accept_visitor(IVisitor visitor) = 0;
	virtual ~IASTNode() = default;
};

typedef shared_ptr<IASTNode> node_ptr;

class CharNode :public IASTNode
{
public:
	CharNode() = delete;
	CharNode(wchar_t ch) :c(ch) {}
	void accept_visitor(IVisitor visitor) override;
private:
	wchar_t c = -2;
};

class RangeNode :public IASTNode
{
public:
	RangeNode() = delete;
	RangeNode(node_ptr node, int min, int max) :node(node), min(min), max(max) {}
	void accept_visitor(IVisitor visitor) override;
private:
	node_ptr node;
	int min;
	int max;	//max=-1 means infinity
};

class SetNode :public IASTNode
{
public:
	SetNode() = delete;
	SetNode(bool ispositive) :ispositive(ispositive) {}
	SetNode &add_set_range(const wchar_t &,const wchar_t &);
	SetNode &add_set_range(const wchar_t &);
	void merge();
	void accept_visitor(IVisitor visitor) override;
private:
	vector<pair<wchar_t, wchar_t>> set;
	bool ispositive = true;
};

class ConcatenationNode :public IASTNode
{
public:
	ConcatenationNode() = delete;
	ConcatenationNode(node_ptr left, node_ptr right) :left(left), right(right) {}
	void accept_visitor(IVisitor visitor) override;
private:
	node_ptr left;
	node_ptr right;
};

class AlternationNode :public IASTNode
{
public:
	AlternationNode() = delete;
	AlternationNode(node_ptr left, node_ptr right) :left(left), right(right) {}
	void accept_visitor(IVisitor visitor) override;
private:
	node_ptr left;
	node_ptr right;
};

class StarNode :public IASTNode
{
public:
	StarNode() = delete;
	StarNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
private:
	node_ptr node;
};

class PlusNode :public IASTNode
{
public:
	PlusNode() = delete;
	PlusNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
private:
	node_ptr node;
};

class QuesNode :public IASTNode
{
public:
	QuesNode() = delete;
	QuesNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
private:
	node_ptr node;
};

