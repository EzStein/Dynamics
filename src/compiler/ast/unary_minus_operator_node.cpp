#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
unary_minus_operator_node::unary_minus_operator_node(expression_node* child) :
unary_operator_node(child) {

}

double unary_minus_operator_node::evaluate() const {
  return -child->evaluate();
}

std::ostream& unary_minus_operator_node::print(std::ostream& out) const {
  out << '-' << '(';
  child->print(out) << ')';
  return out;
}

std::ostream& unary_minus_operator_node::emit_code_ia32(std::ostream& acc, compiler_data& data) const {
  child->emit_code_ia32(acc, data);  //Put on %st(0)
  acc << "fchs %st(0)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xE0';
  return acc;
}

std::ostream& unary_minus_operator_node::emit_code_amd64(std::ostream& acc, compiler_data& data) const {
  child->emit_code_amd64(acc, data);  //Put on %st(0)
  acc << "fchs %st(0)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xE0';
  return acc;
}

unsigned int unary_minus_operator_node::code_size() const {
  return child->code_size() + 2;
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
expression_node* unary_minus_operator_node::transform_negation() {
  expression_node* newChild = child->transform_negation();
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

void unary_minus_operator_node::accept(visitor& v) {
  v.visit(this);
}
