#pragma once
#include <vector>
#include <string>
#include "regex_ast.h"
namespace regex_engine2_parser {

using regex_engine2_ast::node_ptr;
AST regex_parse(std::wstring re);

}