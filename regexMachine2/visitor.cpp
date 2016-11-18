#include "stdafx.h"


namespace regex_engine2_visitor {




void EdgeSetConstructorVisitor::visit(SetNode n)
{
	auto &set = n.get_set();
	for (auto &i : set)
		this->set.add_edge(i);
}


}
