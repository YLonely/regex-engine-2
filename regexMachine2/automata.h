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
public:
	Status() = default;
	Status(std::vector<edge_ptr> in, decltype(in) out) :in_edges(std::move(in)), out_edges(std::move(out)) {}
private:
	void append(Status&);
	std::vector<edge_ptr> in_edges;
	std::vector<edge_ptr> out_edges;
	bool final_status = false;
	int index = -1;

};

typedef std::shared_ptr<Status> status_ptr;

class Edge
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
	friend class Status;
public:
	Edge() = default;
	Edge(status_ptr start, status_ptr end, std::vector<index_t> content = std::vector<index_t>()) :start(std::move(start)), end(std::move(end)), match_content(content) {}
private:
	std::vector<index_t> match_content;
	status_ptr start;
	status_ptr end;

};



class Automata
{
private:
	std::vector<edge_ptr> all_edges;
	std::vector<status_ptr> all_status;
	status_ptr entry;
public:
	inline std::vector<edge_ptr> & get_edges() {
		return all_edges;
	}
	inline std::vector<status_ptr> & get_status() {
		return all_status;
	}
	inline status_ptr &get_entry() {
		return entry;
	}
};



}