#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/number_leaf_node.h"
division_operator_node::division_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double division_operator_node::evaluate() const {
  return leftChild->evaluate() / rightChild->evaluate();
}

long division_operator_node::evaluate_as_integer() const {
  return leftChild->evaluate_as_integer()/rightChild->evaluate_as_integer();
}


std::ostream& division_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '/' << '(';
  rightChild->print(out) << ')';
  return out;
}

void division_operator_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  leftChild->emit_code_amd64(acc, data);  //Put on %st(1)
  rightChild->emit_code_amd64(acc, data); //Now on %st(0)
  acc += "fdivp %st(0), %st(1)\n";
  AST::emit_stack_dec_amd64(acc, data);
}

expression_node* division_operator_node::copy() const {
  return AST::make_binary_operator_node<division_operator_node>(leftChild->copy(), rightChild->copy());
}

/*
 * A division is never guarenteed to be integral.
 */
bool division_operator_node::is_integral() const {
  return false;
}

void division_operator_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* division_operator_node::transform_operators() {
  binary_operator_node::transform_operators();
  expression_node* retVal = new binary_multiplication_operator_node(leftChild,
    new exponentiation_operator_node(rightChild, new integer_number_leaf_node(-1)));
  leftChild = nullptr;
  rightChild = nullptr;
  return retVal;
}


expression_node* division_operator_node::optimization_round() {
  binary_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate_as_integer());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  if(leftChild->evaluatable() && leftChild->is_integral() && leftChild->evaluate_as_integer() == 0) {
    if(rightChild->evaluatable() && rightChild->is_integral() && rightChild->evaluate_as_integer() == 0)
      return this;
    else
      return new integer_number_leaf_node(0);
  } else if(rightChild->evaluatable() && rightChild->is_integral() && rightChild->evaluate_as_integer() == 1) {
    expression_node* tmp = leftChild;
    leftChild = nullptr;
    return tmp;
  } else {
    return this;
  }
}

expression_node* division_operator_node::differentiate(const std::string&) {
  throw "DIVISION NODE IS NOT REQUIRED TO SUPPORT DIFFERENTIATION";
}
