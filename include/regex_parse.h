#pragma once
#include <string>
#include <vector>
#include "regex_ast.h"
namespace lw_regex {
namespace internal_regex_parse {

using lw_regex::internal_ast::node_ptr;
lw_regex::internal_ast::AST regex_parse(std::wstring re);

}  // namespace internal_regex_parse

}  // namespace lw_regex