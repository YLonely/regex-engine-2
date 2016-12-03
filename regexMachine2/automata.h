#pragma once
#include <vector>
#include <memory>
#include "regex.h"
namespace regex_engine2_visitor {
class NFAConstructVisitor;
}


namespace regex_engine2_automata {

using regex_engine2_regex::index_t;
using regex_engine2_regex::index_set;

class Edge;
typedef std::shared_ptr<Edge> edge_ptr;


class Status
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
	friend class Edge;
public:
	Status() = default;
	Status(int index, bool final = false) :index(index), final(final) {}
	auto get_in_edges() {
		return in_edges;
	}
	auto get_out_edges() {
		return out_edges;
	}
	bool is_final() {
		return final;
	}
	int get_index() {
		return index;
	}
private:
	std::vector<edge_ptr> in_edges;
	std::vector<edge_ptr> out_edges;
	bool final = false;
	int index = -1;

};

typedef std::shared_ptr<Status> status_ptr;

class Edge
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
public:
	Edge() = default;
	Edge(std::vector<index_t> content) :match_content(std::move(content)) {}
	auto get_match_content() {
		return match_content;
	}
	auto get_start() {
		return start;
	}
	auto get_end() {
		return end;
	}
private:
	std::vector<index_t> match_content;
	status_ptr start;
	status_ptr end;
};


class DFAStatus
{
public:
	DFAStatus() = default;
	DFAStatus(unsigned int capacity) {
		set_capacity(capacity);
	}
	void set_capacity(unsigned int capacity) {
		status_tran = std::vector<int>(capacity, -1);
	}
	void set_index_set(index_set set) {
		nfa_index = std::move(set);
	}
	bool is_final() {
		return final;
	}
	void set_final() {
		final = true;
	}
	void set_tran(std::vector<int>::size_type index, int value) {
		status_tran[index] = value;
	}
private:
	std::vector<int> status_tran;
	index_set nfa_index;
	bool final = false;
};


class Automata
{
public:
	Automata() = default;
	Automata(status_ptr &start, status_ptr &end) :start(start), end(end) {}
	int get_final_index() {
		return end->get_index();
	}


	std::vector<edge_ptr> all_edges;
	std::vector<status_ptr> all_status;
	status_ptr start;
	status_ptr end;
};



}