#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/polyadic_operator_node.h"
#include "compiler/ast/visitor/mutation/level_multiplication_operator_visitor.h"
binary_multiplication_operator_node::binary_multiplication_operator_node(expression_node* _leftChild, expression_node* _rightChild) :
binary_operator_node(_leftChild, _rightChild) {

}

double binary_multiplication_operator_node::evaluate() const {
  return leftChild->evaluate() * rightChild->evaluate();
}

std::ostream& binary_multiplication_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '*' << '(';
  rightChild->print(out) << ')';
  return out;
}

void binary_multiplication_operator_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  leftChild->emit_code_amd64(acc, data);  //Put on %st(1)
  rightChild->emit_code_amd64(acc, data); //Now on %st(0)
  acc += "fmulp %st(0), %st(1)\n";
  AST::emit_stack_dec_amd64(acc, data);
}

expression_node* binary_multiplication_operator_node::copy() const {
  return AST::make_binary_operator_node<binary_multiplication_operator_node>(leftChild->copy(), rightChild->copy());
}

bool binary_multiplication_operator_node::is_integral() const {
  return leftChild->is_integral() && rightChild->is_integral();
}

void binary_multiplication_operator_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* binary_multiplication_operator_node::level_operators() {
  expression_node* newLeftChild = leftChild->level_operators();
  expression_node* newRightChild = rightChild->level_operators();
  if(leftChild != newLeftChild)
    delete leftChild;
  if(rightChild != newRightChild)
    delete rightChild;

  std::list<expression_node*> newChildren;
  std::list<expression_node*> toDelete;
  level_multiplication_operator_visitor vist(newChildren, toDelete);
  /*Fills new children with the children of the left and right child if the
   child is a multiplication node,
   otherwise it adds the child itself*/
  newLeftChild->accept(&vist);
  newRightChild->accept(&vist);

  for(expression_node* node : toDelete){
    delete node;
  }

  /*Since we are returning a different node, this node will be deleted
   We protected the children by setting them to null*/
  leftChild = nullptr;
  rightChild = nullptr;
  return new polyadic_multiplication_operator_node(newChildren);

}

void binary_multiplication_operator_node::sort() {
  leftChild->sort();
  rightChild->sort();
  if(*leftChild <= *rightChild)
      return;

  /*Otherwise we swap the two*/
  expression_node* tmp = leftChild;
  leftChild = rightChild;
  rightChild = tmp;
}

expression_node* binary_multiplication_operator_node::optimization_round() {
  binary_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  if(leftChild->evaluatable()) {
    if(leftChild->evaluate() == 0) {
      return new integer_number_leaf_node(0);
    } else if(leftChild->evaluate() == 1) {
      expression_node* tmp = rightChild;
      rightChild = nullptr;
      return tmp;
    } else {
      return this;
    }
  } else if(rightChild->evaluatable()) {
    if(rightChild->evaluate() == 0) {
      return new integer_number_leaf_node(0);
    } else if(rightChild->evaluate() == 1) {
      expression_node* tmp = leftChild;
      leftChild = nullptr;
      return tmp;
    } else {
      return this;
    }
  } else {
    return this;
  }
}

expression_node* binary_multiplication_operator_node::differentiate(const std::string&) {
  throw "BINARY MULTIPLICATION NODE IS NOT REQUIRED TO SUPPORT DIFFERENTIATION";
}
