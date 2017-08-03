#include "compiler/ast/divide_operator_node.h"
divide_operator_node::divide_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double divide_operator_node::evaluate() const {
  return leftChild->evaluate() / rightChild->evaluate();
}

std::ostream& divide_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '/' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& divide_operator_node::emit_code(std::ostream& acc, compiler_data& data) const {
  leftChild->emit_code(acc, data);  //Put on %st(1)
  rightChild->emit_code(acc, data); //Now on %st(0)
  acc << "fdivp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xF9';
  return acc;
}

unsigned int divide_operator_node::code_size() const {
  return leftChild->code_size() + rightChild->code_size() + 2;
}
