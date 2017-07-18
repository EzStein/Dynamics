#include "compiler/ast/multiply_operator_node.h"
multiply_operator_node::multiply_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double multiply_operator_node::evaluate() const {
  return leftChild->evaluate() * rightChild->evaluate();
}

std::ostream& multiply_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '*' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& multiply_operator_node::emit_code(std::ostream& acc) const {
  leftChild->emit_code(acc);  //Put on %st(1)
  rightChild->emit_code(acc); //Now on %st(0)
  acc << "fmulp %st(0), %st(1)\n";
  return acc;
}
