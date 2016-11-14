#pragma once
#include <vector>
#include <string>
#include "regex_ast.h"
namespace regex_engine2_parser {

using regex_engine2_astnode::node_ptr;
std::vector<node_ptr> *regex_parse(std::wstring re);

}