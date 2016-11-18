#pragma once
#include <vector>
#include <memory>
#include "regex.h"

namespace regex_engine2_automata {

using regex_engine2_regex::index_t;

struct Edge;
typedef std::shared_ptr<Edge> edge_ptr;

struct Status
{
	std::vector<edge_ptr> in_edges;
	std::vector<edge_ptr> out_edges;
	bool final_status;
};

typedef std::shared_ptr<Status> status_ptr;

struct Edge
{
	std::vector<index_t> match_content;
	status_ptr start;
	status_ptr end;
};

class Automata
{
private:
	std::vector<edge_ptr> all_edges;
	std::vector<status_ptr> all_status;
public:
	inline std::vector<edge_ptr> & get_edges() {
		return all_edges;
	}
	inline std::vector<status_ptr> & get_status() {
		return all_status;
	}
};



}