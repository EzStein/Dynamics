/*
 * Implements the integer number leaf node
 */
#include "compiler/ast/AST.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include <cmath>

integer_number_leaf_node::integer_number_leaf_node(long _val) : val(_val) { }
integer_number_leaf_node::integer_number_leaf_node(int _val) : val(_val) { }

integer_number_leaf_node::integer_number_leaf_node(double _val) {
  val = std::round(_val);
}

void integer_number_leaf_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  acc += "movl " + std::to_string((static_cast<unsigned long>(val) >> 32)) + ", %eax";
  acc += "movl %eax, -0x4c(%rbp)";
  acc += "movl " + std::to_string((static_cast<unsigned long>(val) << 32) >> 32) + ", %eax";
  acc += "movl %eax, -0x50(%rbp)";
  AST::emit_stack_inc_amd64(acc, data);
  "fldl -0x50(%rbp)\n";
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

bool integer_number_leaf_node::is_integral() const {
  return true;
}

void integer_number_leaf_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* integer_number_leaf_node::differentiate(const std::string& var) {
  return new integer_number_leaf_node(0);
}
