#include "visitor.h"
#include "std.h"

namespace lw_regex {
namespace internal_visitor {

std::vector<edge_ptr> all_edges;
std::vector<status_ptr> all_status;
void record(edge_ptr& e) {
    all_edges.push_back(e);
}

void record(status_ptr& s) {
    all_status.push_back(s);
}

Automata NFAConstructVisitor::connect(Automata& a, status_ptr& s) {
    edge_ptr epsilon_edge = make_shared<Edge>();
    epsilon_edge->start = a.end;
    epsilon_edge->end = s;
    a.end->out_edges.push_back(epsilon_edge);
    s->in_edges.push_back(epsilon_edge);

    record(epsilon_edge);
    return Automata(a.start, s);
}

void NFAConstructVisitor::connect(status_ptr& start,
                                  status_ptr& end,
                                  edge_ptr e = make_shared<Edge>()) {
    e->start = start;
    e->end = end;
    start->out_edges.push_back(e);
    end->in_edges.push_back(e);
    record(e);
}

Automata NFAConstructVisitor::connect(Automata& start, Automata& end) {
    if (!end.start)
        return start;
    if (!start.start)
        return end;
    connect(start.end, end.start);
    return Automata(start.start, end.end);
}

Automata NFAConstructVisitor::apply(CharNode* n) {
    status_ptr start = make_shared<NFAStatus>();
    status_ptr end = make_shared<NFAStatus>();
    edge_ptr char_edge =
        make_shared<Edge>(set.get_group_index({n->get_char(), n->get_char()}));
    connect(start, end, char_edge);

    record(start);
    record(end);
    // record(char_edge);
    return Automata(start, end);
}

Automata NFAConstructVisitor::apply(ConcatenationNode* n) {
    Automata left = invoke(n->get_left());
    Automata right = invoke(n->get_right());
    return connect(left, right);
}

Automata NFAConstructVisitor::apply(RangeNode* n) {
    Automata head;
    int min = n->get_range().first, max = n->get_range().second;
    if (min == 0 && max == -1) {
        auto start = make_shared<NFAStatus>();
        record(start);
        auto frag = invoke(n->get_node());
        auto end = make_shared<NFAStatus>();
        connect(frag.end, frag.start);
        connect(frag.end, end);

        connect(start, frag.start);
        connect(start, end);

        record(end);
        return Automata(start, end);
    } else {
        if (max == -1) {
            for (int i = 0; i < min - 1; ++i) {
                auto frag = invoke(n->get_node());
                head = connect(head, frag);
            }
            auto end = invoke(n->get_node());
            connect(end.end, end.start);
            return connect(head, end);
        }
        for (int i = 0; i < min; ++i) {
            auto frag = invoke(n->get_node());
            head = connect(head, frag);
        }
        std::vector<status_ptr> frag_starts;
        for (int i = 0; i < max - min; ++i) {
            auto frag = invoke(n->get_node());
            frag_starts.push_back(frag.start);
            head = connect(head, frag);
        }
        status_ptr end = make_shared<NFAStatus>();
        record(end);
        for (auto& it : frag_starts)
            connect(it, end);
        return connect(head, end);
    }
}

Automata NFAConstructVisitor::apply(SetNode* n) {
    status_ptr start = make_shared<NFAStatus>();
    status_ptr end = make_shared<NFAStatus>();
    edge_ptr char_edge = make_shared<Edge>(set.get_group_index(n->get_set()));
    connect(start, end, char_edge);

    record(start);
    record(end);
    // record(char_edge);
    return Automata(start, end);
}

Automata NFAConstructVisitor::apply(AlternationNode* n) {
    auto alter_status = make_shared<NFAStatus>();

    record(alter_status);

    Automata left = invoke(n->get_left());
    Automata right = invoke(n->get_right());
    auto end_status = make_shared<NFAStatus>();
    connect(alter_status, left.start);
    connect(alter_status, right.start);

    connect(left.end, end_status);
    connect(right.end, end_status);

    record(end_status);
    return Automata(alter_status, end_status);
    // return Automata();
}

Automata NFAConstructVisitor::apply(StarNode* n) {
    auto start = make_shared<NFAStatus>();
    record(start);
    auto frag = invoke(n->get_node());
    auto end = make_shared<NFAStatus>();
    connect(frag.end, frag.start);
    connect(frag.end, end);

    connect(start, frag.start);
    connect(start, end);

    record(end);
    return Automata(start, end);
}

Automata NFAConstructVisitor::apply(PlusNode* n) {
    auto frag = invoke(n->get_node());
    connect(frag.end, frag.start);
    return Automata(frag.start, frag.end);
}

Automata NFAConstructVisitor::apply(QuesNode* n) {
    auto frag = invoke(n->get_node());
    connect(frag.start, frag.end);
    return Automata(frag.start, frag.end);
}

Automata NFAConstructVisitor::apply(EndOfString* end) {
    NFAStatus::reset();
    Automata nfa = invoke(end->get_node());
    nfa.end->final = true;
    nfa.all_edges = std::move(all_edges);
    nfa.all_status = std::move(all_status);
    return nfa;
}

}  // namespace internal_visitor
}  // namespace lw_regex
