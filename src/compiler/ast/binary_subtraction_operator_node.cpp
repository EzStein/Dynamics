#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/binary_addition_operator_node.h"
#include "binary_multiplication_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
binary_subtraction_operator_node::binary_subtraction_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double binary_subtraction_operator_node::evaluate() const {
  return leftChild->evaluate() - rightChild->evaluate();
}

long binary_subtraction_operator_node::evaluate_as_integer() const {
  return leftChild->evaluate_as_integer() - rightChild->evaluate_as_integer();
}


std::ostream& binary_subtraction_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '-' << '(';
  rightChild->print(out) << ')';
  return out;
}

void binary_subtraction_operator_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  leftChild->emit_code_amd64(acc, data);  //Put on %st(1)
  rightChild->emit_code_amd64(acc, data); //Now on %st(0)
  acc += "fsubp %st(0), %st(1)\n";
  AST::emit_stack_dec_amd64(acc, data);
}

expression_node* binary_subtraction_operator_node::copy() const {
  return AST::make_binary_operator_node<binary_subtraction_operator_node>(leftChild->copy(), rightChild->copy());
}

bool binary_subtraction_operator_node::is_integral() const {
  return leftChild->is_integral() && rightChild->is_integral();
}

/*
 * Transforms a - b into a + -1*b
 */
expression_node* binary_subtraction_operator_node::transform_operators() {
  expression_node* newLeftChild = leftChild->transform_operators();
  expression_node* newRightChild = rightChild->transform_operators();
  if(newLeftChild != leftChild)
    delete leftChild;
  if(newRightChild != rightChild)
    delete rightChild;

  binary_addition_operator_node* newNode =
    new binary_addition_operator_node(newLeftChild,
    new binary_multiplication_operator_node(new integer_number_leaf_node(-1),
    newRightChild));

  /*Since we don't return this, this node
   will be deleted by the calling function.
   We do not want the destructor to delete the children, which
   may be used in the newNode, so we set them to null*/
  leftChild = nullptr;
  rightChild = nullptr;
  return newNode;
}

void binary_subtraction_operator_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* binary_subtraction_operator_node::optimization_round() {
  binary_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate_as_integer());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  if(leftChild->evaluatable() && leftChild->is_integral() && leftChild->evaluate_as_integer() == 0) {
    expression_node* tmp = rightChild;
    rightChild = nullptr;
    return new unary_minus_operator_node(tmp);
  } else if(rightChild->evaluatable() && rightChild->is_integral() && rightChild->evaluate_as_integer() == 0) {
    expression_node* tmp = leftChild;
    leftChild = nullptr;
    return tmp;
  } else {
    return this;
  }
}

expression_node* binary_subtraction_operator_node::differentiate(const std::string&) {
  throw "BINARY SUBTRACTION NODE IS NOT REQUIRED TO SUPPORT DIFFERENTIATION";
}
