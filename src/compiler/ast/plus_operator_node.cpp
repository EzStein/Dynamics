#include "compiler/ast/plus_operator_node.h"
#include <iostream>
plus_operator_node::plus_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double plus_operator_node::evaluate() const {
  return leftChild->evaluate() + rightChild->evaluate();
}

std::ostream& plus_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '+' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& plus_operator_node::emit_code(std::ostream& acc, compiler_data& data) const {
  leftChild->emit_code(acc, data);  //Put on %st(1)
  rightChild->emit_code(acc, data); //Now on %st(0)

  acc << "faddp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xC1';

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

unsigned int plus_operator_node::code_size() const {
  return leftChild->code_size() + rightChild->code_size() + 13;
}
