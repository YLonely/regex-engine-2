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
	virtual void operation() = 0;
};

typedef shared_ptr<IASTNode> node_ptr;

class CharNode :public IASTNode
{
public:
	CharNode() = delete;
	CharNode(char ch, bool is_functionalchar) :c(ch), is_functionalchar(is_functionalchar) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	char c = -2;
	bool is_functionalchar = false;
};

class RangeNode :public IASTNode
{
public:
	RangeNode() = delete;
	RangeNode(node_ptr node, int min, int max) :node(node), min(min), max(max) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
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
	void add_set_range(pair<char, char> &p);
	void add_set_range(char &ch);
	void merge();
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	vector<pair<char, char>> set;
	bool ispositive = true;
};

class ConcatenationNode :public IASTNode
{
public:
	ConcatenationNode() = delete;
	ConcatenationNode(node_ptr left, node_ptr right) :left(left), right(right) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
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
	void operation() override;
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
	void operation() override;
private:
	node_ptr node;
};

class PlusNode :public IASTNode
{
public:
	PlusNode() = delete;
	PlusNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	node_ptr node;
};

class QuesNode :public IASTNode
{
public:
	QuesNode() = delete;
	QuesNode(node_ptr node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	node_ptr node;
};

void regex_parse(const string &re);