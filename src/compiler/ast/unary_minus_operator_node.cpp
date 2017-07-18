#include "compiler/ast/unary_minus_operator_node.h"
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

std::ostream& unary_minus_operator_node::emit_code(std::ostream& acc) const {
  child->emit_code(acc);  //Put on %st(0)
  acc << "fchs %st(0)\n";
  return acc;
}
