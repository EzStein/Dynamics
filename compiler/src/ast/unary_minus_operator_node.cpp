#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
unary_minus_operator_node::unary_minus_operator_node(expression_node* child) :
unary_operator_node(child) {

}

double unary_minus_operator_node::evaluate() const {
  return -child->evaluate();
}

long unary_minus_operator_node::evaluate_as_integer() const {
  return -child->evaluate_as_integer();
}


std::ostream& unary_minus_operator_node::print(std::ostream& out) const {
  out << '-' << '(';
  child->print(out) << ')';
  return out;
}

void unary_minus_operator_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  child->emit_code_amd64(acc, data);  //Put on %st(0)
  acc += "fchs %st(0)\n";
}

expression_node* unary_minus_operator_node::copy() const {
  return AST::make_unary_operator_node<unary_minus_operator_node>(child->copy());
}

bool unary_minus_operator_node::is_integral() const {
  return child->is_integral();
}

/*
 * Transforms -a into -1 * a
 */
expression_node* unary_minus_operator_node::transform_operators() {
  expression_node* newChild = child->transform_operators();
  /*The method decided that itself was needed to change so we delete it*/
  if(newChild != child)
    delete child;
  expression_node* newNode = new binary_multiplication_operator_node(new integer_number_leaf_node(-1), newChild);
  /*Since we don't return this, this node
   will be deleted by the calling function.
   We do not want the destructor to delete the children, which
   may be used in the newNode, so we set them to null*/
  child = nullptr;
  return newNode;
}

void unary_minus_operator_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* unary_minus_operator_node::optimization_round() {
  unary_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate_as_integer());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  if(child->evaluatable()) {
    if(child->is_integral()) {
      return new integer_number_leaf_node(-1 * child->evaluate_as_integer());
    } else {
      return new number_leaf_node(-1 * child->evaluate());
    }
  } else {
    return this;
  }
}

expression_node* unary_minus_operator_node::differentiate(const std::string&) {
  throw "UNARY MINUS OPERATOR NOT REQUIRED TO IMPLEMENT DIFFERENTIATION";
}
