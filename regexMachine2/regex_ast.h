#pragma once
#include <utility>
#include <vector>
using namespace std;
class IVisitor;

class IASTNode
{
public:
	virtual void accept_visitor(IVisitor visitor) = 0;
	virtual ~IASTNode() = default;
	virtual void operation() = 0;
};

class CharNode :public IASTNode
{
public:
	CharNode() = delete;
	CharNode(char ch) :c(ch) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	char c = -2;
};

class RangeNode :public IASTNode
{
public:
	RangeNode() = delete;
	RangeNode(IASTNode *node, int min, int max) :node(node), min(min), max(max) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	IASTNode *node;
	int min;
	int max;	//max=-1 means infinity
};

class SetNode :public IASTNode
{
public:
	SetNode() = default;
	void add_set_range(pair<char, char> p);
	void add_set_range(char ch);
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	vector<pair<char, char>> set;
};

class ConcatenationNode :public IASTNode
{
public:
	ConcatenationNode() = delete;
	ConcatenationNode(IASTNode *left, IASTNode *right) :left(left), right(right) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	IASTNode *left;
	IASTNode *right;
};

class AlternationNode :public IASTNode
{
public:
	AlternationNode() = delete;
	AlternationNode(IASTNode *left, IASTNode *right) :left(left), right(right) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	IASTNode *left;
	IASTNode *right;
};

class StarNode :public IASTNode
{
public:
	StarNode() = delete;
	StarNode(IASTNode *node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	IASTNode *node;
};

class PlusNode :public IASTNode
{
public:
	PlusNode() = delete;
	PlusNode(IASTNode *node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	IASTNode *node;
};

class QuesNode :public IASTNode
{
public:
	QuesNode() = delete;
	QuesNode(IASTNode *node) :node(node) {}
	void accept_visitor(IVisitor visitor) override;
	void operation() override;
private:
	IASTNode *node;
};

