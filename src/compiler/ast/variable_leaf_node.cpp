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

std::ostream& variable_leaf_node::emit_code(std::ostream& acc, compiler_data& data) const {
  acc << "fldl 8(%ebp)\n";
  data.executableBuf[++data.offset] = '\xDD';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\x08';
  return acc;
}

unsigned int variable_leaf_node::code_size() const {
  return 3;
}
