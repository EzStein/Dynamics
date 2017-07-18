#include "compiler/ast/number_leaf_node.h"
number_leaf_node::number_leaf_node(double _val) : val(_val) {

}

double number_leaf_node::evaluate() const {
  return val;
}

std::ostream& number_leaf_node::print(std::ostream& out) const {
  out << val;
  return out;
}

std::ostream& number_leaf_node::emit_code(std::ostream& acc) const {
  acc << "fldl " << &val << "\n";
  return acc;
}
