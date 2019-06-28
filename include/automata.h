#pragma once
#include <list>
#include <memory>
#include <set>
#include <vector>
#include "regex_exception.h"

namespace lw_regex {
namespace internal_visitor {
class NFAConstructVisitor;
}  // namespace internal_visitor
}  // namespace lw_regex

namespace lw_regex {
namespace internal_automata {

typedef std::pair<wchar_t, wchar_t> char_group;
typedef std::vector<char_group>::size_type group_index;
using lw_regex::regex_runtime_error;

class Edge;
class Automata;
typedef std::shared_ptr<Edge> edge_ptr;
typedef int nfa_index;
typedef int dfa_index;

class NFAStatus {
    friend class lw_regex::internal_visitor::NFAConstructVisitor;
    friend class Edge;
    friend class Automata;

   public:
    NFAStatus(bool final = false) : final(final) { index = index_count++; }
    inline std::vector<edge_ptr> get_in_edges() { return in_edges; }
    inline std::vector<edge_ptr> get_out_edges() { return out_edges; }
    inline bool is_final() { return final; }
    inline int get_index() { return index; }
    static void reset() { index_count = 0; }

   private:
    std::vector<edge_ptr> in_edges;
    std::vector<edge_ptr> out_edges;
    bool final = false;
    nfa_index index;
    static nfa_index index_count;
};

typedef std::shared_ptr<NFAStatus> status_ptr;

class Edge {
    friend class lw_regex::internal_visitor::NFAConstructVisitor;
    friend class Automata;

   public:
    Edge() = default;
    Edge(std::vector<group_index> content)
        : match_content(std::move(content)) {}
    std::vector<group_index>& get_match_content() { return match_content; }
    status_ptr get_start() { return start; }
    status_ptr get_end() { return end; }

   private:
    std::vector<group_index> match_content;
    status_ptr start;
    status_ptr end;
};

// The list that stores the nfa status;
typedef std::list<status_ptr> status_set;
// The set that stores the index of some nfa status
typedef std::set<nfa_index> index_set;

class DFAStatus {
   public:
    DFAStatus() = default;
    DFAStatus(index_set s, bool final = false)
        : contain_nfa(std::move(s)), final(final) {
        if (char_group_size == 0)
            throw regex_runtime_error(L"DFAStatus:capacity==0");
        status_tran = std::vector<dfa_index>(char_group_size, -1);
        index = index_count++;
    }
    static unsigned int size() { return char_group_size; }
    static void set_size(unsigned int c) { char_group_size = c; }
    void set_nfa_set(index_set set) { contain_nfa = std::move(set); }
    index_set& get_nfa_set() { return contain_nfa; }
    bool is_final() { return final; }
    dfa_index& operator[](std::vector<dfa_index>::size_type n) {
        if (n >= status_tran.size())
            return INDEX_ERROR;
        return status_tran[n];
    }
    const dfa_index& operator[](std::vector<dfa_index>::size_type n) const {
        if (n >= status_tran.size())
            return INDEX_ERROR;
        return status_tran[n];
    }
    dfa_index get_index() { return index; }
    static void reset() { index_count = 0; }

   private:
    std::vector<dfa_index> status_tran;
    index_set contain_nfa;
    bool final = false;
    dfa_index index;
    static unsigned int char_group_size;
    static dfa_index index_count;
    static int INDEX_ERROR;
};

typedef std::vector<DFAStatus> Dtran;

class Automata {
   public:
    Automata() = default;
    Automata(status_ptr& start, status_ptr& end) : start(start), end(end) {}
    int get_final_index() { return end->get_index(); }
    void free() {
        for (auto& edge : all_edges) {
            edge->start.reset();
            edge->end.reset();
        }
        for (auto& s : all_status) {
            s->in_edges.clear();
            s->out_edges.clear();
        }
        start.reset();
        end.reset();
        all_edges.clear();
        all_status.clear();
    }
    std::vector<edge_ptr> all_edges;
    std::vector<status_ptr> all_status;
    status_ptr start;
    status_ptr end;
};

}  // namespace internal_automata

}  // namespace lw_regex
