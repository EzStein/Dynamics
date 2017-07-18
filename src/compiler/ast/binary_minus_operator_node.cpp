#include "compiler/ast/binary_minus_operator_node.h"
binary_minus_operator_node::binary_minus_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double binary_minus_operator_node::evaluate() const {
  return leftChild->evaluate() - rightChild->evaluate();
}

std::ostream& binary_minus_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '-' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& binary_minus_operator_node::emit_code(std::ostream& acc) const {
  leftChild->emit_code(acc);  //Put on %st(1)
  rightChild->emit_code(acc); //Now on %st(0)
  acc << "fsubp %st(0), %st(1)\n";
  return acc;
}
