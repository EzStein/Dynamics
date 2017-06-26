#include "compiler/ast/plus_operator_node.h"
#include <iostream>
plus_operator_node::plus_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double plus_operator_node::evaluate() const {
  return leftChild->evaluate() + rightChild->evaluate();
}

std::ostream& plus_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '+' << '(';
  rightChild->print(out) << ')';
  return out;
}
