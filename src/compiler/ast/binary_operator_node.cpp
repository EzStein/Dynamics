#include "compiler/ast/binary_operator_node.h"
#include "compiler/ast/expression_node.h"
binary_operator_node::binary_operator_node(expression_node* _leftChild, expression_node* _rightChild) :
leftChild(_leftChild), rightChild(_rightChild) {
}

binary_operator_node::~binary_operator_node() {
  delete leftChild;
  delete rightChild;
}

bool binary_operator_node::evaluatable() const {
  return leftChild->evaluatable() && rightChild->evaluatable();
}

expression_node* binary_operator_node::transform_negation() {
  expression_node* newChild = leftChild->transform_negation();
  if(newChild != leftChild)
    delete leftChild;
  leftChild = newChild;
  newChild = rightChild->transform_negation();
  if(newChild != rightChild)
    delete rightChild;
  rightChild = newChild;
  return this;
}

/*expression_node* binary_operator_node::level_operators(unsigned int& type,
  polyadic_operator_node::const_iterator_t& start, polyadic_operator_node::const_iterator_t& end) {
  expression_node* newChild = leftChild->level_operators(type, start, end);
  if(newChild != leftChild)
    delete leftChild;
  leftChild = newChild;
  newChild = rightChild->level_operators(type, start, end);
  if(newChild != rightChild)
    delete rightChild;
  rightChild = newChild;
  return this;
}*/
