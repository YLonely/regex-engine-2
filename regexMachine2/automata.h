#pragma once
#include <vector>
#include <memory>
#include <set>
#include <list>
#include "regex.h"
namespace regex_engine2_visitor {
class NFAConstructVisitor;
}


namespace regex_engine2_automata {

using regex_engine2_regex::group_index;


class Edge;
typedef std::shared_ptr<Edge> edge_ptr;
typedef int nfa_index;
typedef int dfa_index;

class NFAStatus
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
	friend class Edge;
public:
	NFAStatus() = default;
	NFAStatus(int index, bool final = false) :index(index), final(final) {}
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
	nfa_index index = -1;

};

typedef std::shared_ptr<NFAStatus> status_ptr;

class Edge
{
	friend class regex_engine2_visitor::NFAConstructVisitor;
public:
	Edge() = default;
	Edge(std::vector<group_index> content) :match_content(std::move(content)) {}
	auto &get_match_content() {
		return match_content;
	}
	auto get_start() {
		return start;
	}
	auto get_end() {
		return end;
	}
private:
	std::vector<group_index> match_content;
	status_ptr start;
	status_ptr end;
};


//The list that stores the nfa status;
typedef std::list<status_ptr> status_set;
//The set that stores the index of some nfa status
typedef std::set<nfa_index> index_set;

class DFAStatus
{
public:
	DFAStatus() = default;
	DFAStatus(index_set s, bool final = false) :
		contain_nfa(std::move(s)), final(final) {
		if (capacity == 0)
			throw std::runtime_error("DFAStatus:capacity==0");
		status_tran = std::vector<dfa_index>(capacity, -1);
		index = index_count++;
	}
	static unsigned int get_capacity() {
		return capacity;
	}
	static void set_capacity(unsigned int c) {
		capacity = c;
	}
	void set_nfa_set(index_set set) {
		contain_nfa = std::move(set);
	}
	auto get_nfa_set() const {
		return contain_nfa;
	}
	bool is_final() {
		return final;
	}
	void set_tran(std::vector<dfa_index>::size_type index, int value) {
		status_tran[index] = value;
	}
	dfa_index tran(std::vector<dfa_index>::size_type index) {
		return status_tran[index];
	}
	dfa_index get_index() {
		return index;
	}
	static void reset() {
		index_count = 0;
	}
private:
	std::vector<dfa_index> status_tran;
	index_set contain_nfa;
	bool final = false;
	dfa_index index;
	static unsigned int capacity;
	static dfa_index index_count;
};



typedef std::vector<DFAStatus> Dtran;



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