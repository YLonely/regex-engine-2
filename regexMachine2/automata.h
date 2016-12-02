#pragma once
#include <vector>
#include <memory>
#include "regex.h"
namespace regex_engine2_visitor {
class NFAConstructVisitor;
}


namespace regex_engine2_automata {

using regex_engine2_regex::index_t;

class Edge;
typedef std::shared_ptr<Edge> edge_ptr;



class Status
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
	friend class Edge;
public:
	Status() = default;
	Status(int index, bool final = false) :index(index), is_final(final) {}
private:
	std::vector<edge_ptr> in_edges;
	std::vector<edge_ptr> out_edges;
	bool is_final = false;
	int index = -1;

};

typedef std::shared_ptr<Status> status_ptr;

class Edge
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
public:
	Edge() = default;
	Edge(std::vector<index_t> content) :match_content(std::move(content)) {}
private:
	std::vector<index_t> match_content;
	status_ptr start;
	status_ptr end;
};



class Automata
{
public:
	Automata() = default;
	Automata(status_ptr &start, status_ptr &end) :start(start), end(end) {}
	std::vector<edge_ptr> all_edges;
	std::vector<status_ptr> all_status;
	status_ptr start;
	status_ptr end;
};



}