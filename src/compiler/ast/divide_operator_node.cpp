#include "compiler/ast/divide_operator_node.h"
divide_operator_node::divide_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double divide_operator_node::evaluate() const {
  return leftChild->evaluate() / rightChild->evaluate();
}

std::ostream& divide_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '/' << '(';
  rightChild->print(out) << ')';
  return out;
}
