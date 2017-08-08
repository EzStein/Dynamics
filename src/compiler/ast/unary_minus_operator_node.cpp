#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/AST.h"
unary_minus_operator_node::unary_minus_operator_node(expression_node* child) :
unary_operator_node(child) {

}

double unary_minus_operator_node::evaluate() const {
  return -child->evaluate();
}

std::ostream& unary_minus_operator_node::print(std::ostream& out) const {
  out << '-' << '(';
  child->print(out) << ')';
  return out;
}

std::ostream& unary_minus_operator_node::emit_code(std::ostream& acc, compiler_data& data) const {
  child->emit_code(acc, data);  //Put on %st(0)
  acc << "fchs %st(0)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xE0';
  return acc;
}

unsigned int unary_minus_operator_node::code_size() const {
  return child->code_size() + 2;
}

expression_node* unary_minus_operator_node::copy() const {
  return AST::make_unary_operator_node<unary_minus_operator_node>(child->copy());
}
