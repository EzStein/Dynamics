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
