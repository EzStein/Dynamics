#include "compiler/ast/variable_leaf_node.h"
#include "compiler/ast/AST.h"
variable_leaf_node::variable_leaf_node(symbol::ptr_type _symPtr):symPtr(_symPtr) {

}

bool variable_leaf_node::evaluatable() const {
  return false;
}

double variable_leaf_node::evaluate() const {
  return static_cast<double>(0);
}

long variable_leaf_node::evaluate_as_integer() const {
  return 0;
}

std::ostream& variable_leaf_node::print(std::ostream& out) const {
  out << symPtr->name;
  return out;
}

void variable_leaf_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  AST::emit_stack_inc_amd64(acc, data);
  unsigned int offset;
  if(symPtr->parameter <= 7) {
    offset = reinterpret_cast<unsigned int>(-72 + symPtr->parameter*8);
  } else {
    offset = 16 + 8 * (symPtr->parameter - 8);
  }

  acc += "fldl " +  std::to_string(offset) + "(%rbp)\n";
}

expression_node* variable_leaf_node::copy() const {
  return AST::make_variable_leaf_node(symPtr);
}

bool variable_leaf_node::is_integral() const {
  return false;
}

void variable_leaf_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* variable_leaf_node::differentiate(const std::string& var) {
  if(symPtr->name == var)
    return new integer_number_leaf_node(1);
  else
    return new integer_number_leaf_node(0);
}
