#include <cmath>
#include "exponentiation_operator_node.h"
exponentiation_operator_node::exponentiation_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double exponentiation_operator_node::evaluate() const {
  return pow(leftChild->evaluate(), rightChild->evaluate());
}
