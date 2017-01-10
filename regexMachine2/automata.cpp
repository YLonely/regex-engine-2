#include "stdafx.h"
#include "automata.h"


namespace regex_engine2_automata {
using std::make_shared;
dfa_index DFAStatus::index_count;
unsigned int DFAStatus::char_group_size;
nfa_index NFAStatus::index_count;
int DFAStatus::INDEX_ERROR = -1;
}

