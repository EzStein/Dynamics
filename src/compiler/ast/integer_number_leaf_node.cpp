/*
 * Implements the integer number leaf node
 */
#include "compiler/ast/integer_number_leaf_node.h"

integer_number_leaf_node::integer_number_leaf_node(long _val) : val(_val) { }

std::ostream& integer_number_leaf_node::emit_code_ia32(std::ostream& out, compiler_data&) const {
  return out;
}

std::ostream& integer_number_leaf_node::emit_code_amd64(std::ostream& out, compiler_data&) const {
  return out;
}

double integer_number_leaf_node::evaluate() const {
  return static_cast<double>(val);
}

/*True if there are no variables in the sub tree whose root is this node*/
bool integer_number_leaf_node::evaluatable() const {
  return true;
}

std::ostream& integer_number_leaf_node::print(std::ostream& out) const {
  out << val;
  return out;
}

/*
* Copies the tree whose root is this node and returns a pointer to this copy
*/
expression_node* integer_number_leaf_node::copy() const {
  return new integer_number_leaf_node(val);
}

/*
* Returns the maximum code size of the produced code.
*/
unsigned int integer_number_leaf_node::code_size() const {
  return 100;
}

bool integer_number_leaf_node::is_integral() const {
  return true;
}

void integer_number_leaf_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* integer_number_leaf_node::differentiate(const std::string& var) {
  return new integer_number_leaf_node(0);
}
