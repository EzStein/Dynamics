#include "compiler/ast/binary_operator_node.h"
binary_operator_node::binary_operator_node(expression_node* _leftChild, expression_node* _rightChild) :
leftChild(_leftChild), rightChild(_rightChild) {
}

binary_operator_node::~binary_operator_node() { }

bool binary_operator_node::evaluatable() const {
  return leftChild->evaluatable() && rightChild->evaluatable();
}
