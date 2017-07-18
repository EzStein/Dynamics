#include <cmath>
#include "exponentiation_operator_node.h"
exponentiation_operator_node::exponentiation_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double exponentiation_operator_node::evaluate() const {
  return pow(leftChild->evaluate(), rightChild->evaluate());
}

std::ostream& exponentiation_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '^' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& exponentiation_operator_node::emit_code(std::ostream& acc) const {
  leftChild->emit_code(acc);  //Put on %st(1)
  rightChild->emit_code(acc); //Now on %st(0)
  acc << "EXP\n";
  return acc;
}
