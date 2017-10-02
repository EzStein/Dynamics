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

expression_node* binary_operator_node::level_operators() {
  expression_node* newChild = leftChild->level_operators();
  /*If the leftChild has been replaced by a new node, we delete the left child*/
  if(newChild != leftChild)
    delete leftChild;
  leftChild = newChild;

  newChild = rightChild->level_operators();
  if(newChild != rightChild)
    delete rightChild;
  rightChild = newChild;

  /*The operators have been leveled so we do not need to change this node*/
  return this;
}

/*Left unimplemented because binary operators are not present when this method is called*/
expression_node* binary_operator_node::make_pre_canonical() {
  throw "Attempt to traverse a binary operator during pre-canonical formation";
  return nullptr;
}

void binary_operator_node::sort() {
  /*By default this method just calls sort on its children. 
   commutative operators will implement ordering*/
  leftChild->sort();
  rightChild->sort();
}

expression_node* binary_operator_node::collect_terms() {
  throw "NON EXPONENTIATION BINARY OPERATORS ARE NOT REQUIRED TO IMPLEMENT COLLECT_TERMS";
}

expression_node* binary_operator_node::optimization_round() {
  expression_node* tmp = leftChild->optimization_round();
  if(tmp != leftChild)
    delete leftChild;
  leftChild = tmp;
  tmp = rightChild->optimization_round();
  if(tmp != rightChild)
    delete rightChild;
  rightChild = tmp;
  
  return this;
}
