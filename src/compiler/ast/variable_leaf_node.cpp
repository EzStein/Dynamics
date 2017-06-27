#include "compiler/ast/variable_leaf_node.h"
variable_leaf_node::variable_leaf_node(symbol_ptr_type _symbolPtr):symbolPtr(_symbolPtr) {

}

double variable_leaf_node::evaluate() const {
  return static_cast<double>(0);
}

std::ostream& variable_leaf_node::print(std::ostream& out) const {
  out << symbolPtr->name;
  return out;
}
