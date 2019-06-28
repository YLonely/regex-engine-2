#include "automata.h"
#include "std.h"

namespace lw_regex {
namespace internal_automata {

using std::make_shared;
dfa_index DFAStatus::index_count;
unsigned int DFAStatus::char_group_size;
nfa_index NFAStatus::index_count;
int DFAStatus::INDEX_ERROR = -1;
}  // namespace internal_automata
}  // namespace lw_regex
