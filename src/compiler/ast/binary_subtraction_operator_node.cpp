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

std::ostream& binary_subtraction_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '-' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& binary_subtraction_operator_node::emit_code_ia32(std::ostream& acc, compiler_data& data) const {
  leftChild->emit_code_ia32(acc, data);  //Put on %st(1)
  rightChild->emit_code_ia32(acc, data); //Now on %st(0)
  acc << "fsubp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xE9';

  if(data.stackSizeFPU >= 9) {
    acc << "fldt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x2C';
    data.executableBuf[++data.offset] = '\x24';

    acc << "addl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xC4';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fincstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF7';
  }
  --data.stackSizeFPU;
  return acc;
}

std::ostream& binary_subtraction_operator_node::emit_code_amd64(std::ostream& acc, compiler_data& data) const {
  leftChild->emit_code_amd64(acc, data);  //Put on %st(1)
  rightChild->emit_code_amd64(acc, data); //Now on %st(0)
  acc << "fsubp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xE9';

  AST::emit_stack_dec_amd64(acc, data);
  return acc;
}

unsigned int binary_subtraction_operator_node::code_size() const {
  return leftChild->code_size() + rightChild->code_size() + 13;
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
expression_node* binary_subtraction_operator_node::transform_negation() {
  expression_node* newLeftChild = leftChild->transform_negation();
  expression_node* newRightChild = rightChild->transform_negation();
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
