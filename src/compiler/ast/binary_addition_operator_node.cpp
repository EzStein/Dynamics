#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/visitor/mutation/level_addition_operator_visitor.h"
#include <iostream>
#include <list>
binary_addition_operator_node::binary_addition_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double binary_addition_operator_node::evaluate() const {
  return leftChild->evaluate() + rightChild->evaluate();
}

std::ostream& binary_addition_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '+' << '(';
  rightChild->print(out) << ')';
  return out;
}

void binary_addition_operator_node::emit_code_amd64(std::string& str, compiler_data& data) const {
  leftChild->emit_code_amd64(str, data);  //Put on %st(1)
  rightChild->emit_code_amd64(str, data); //Now on %st(0)

  str += "faddp %st(0), %st(1)\n";

  AST::emit_stack_dec_amd64(str, data);
}

expression_node* binary_addition_operator_node::copy() const {
  return AST::make_binary_operator_node<binary_addition_operator_node>(leftChild->copy(), rightChild->copy());
}

bool binary_addition_operator_node::is_integral() const {
  return leftChild->is_integral() && rightChild->is_integral();
}

void binary_addition_operator_node::accept(visitor* vist) {
  vist->visit(this);
}

expression_node* binary_addition_operator_node::level_operators() {
  expression_node* newLeftChild = leftChild->level_operators();
  expression_node* newRightChild = rightChild->level_operators();
  if(leftChild != newLeftChild)
    delete leftChild;
  if(rightChild != newRightChild)
    delete rightChild;

  std::list<expression_node*> newChildren;
  std::list<expression_node*> toDelete;
  level_addition_operator_visitor vist(newChildren, toDelete);
  /*Fills new children with the children of the left and right child if the
   child is an addition node,
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
  return new polyadic_addition_operator_node(newChildren);
}

void binary_addition_operator_node::sort() {
  leftChild->sort();
  rightChild->sort();
  if(*leftChild <= *rightChild)
    return;

  /*Otherwise we swap the two*/
  expression_node* tmp = leftChild;
  leftChild = rightChild;
  rightChild = tmp;
}

expression_node* binary_addition_operator_node::optimization_round() {
  binary_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate());
    } else {
      return new number_leaf_node(evaluate());
    }
  }

  expression_node* retVal;
  if(leftChild->evaluatable() && leftChild->evaluate() == 0) {
    retVal = rightChild;
    /*This node will be deleted but we don't want the rightchild to be deleted*/
    rightChild = nullptr;
  } else if(rightChild->evaluatable() && rightChild->evaluate() == 0) {
    retVal = leftChild;
    leftChild = nullptr;
  } else {
    retVal = this;
  }
  return retVal;
}

expression_node* binary_addition_operator_node::differentiate(const std::string&) {
  throw "BINARY ADDITION NODE IS NOT REQUIRED TO SUPPORT DIFFERENTIATION";
}
