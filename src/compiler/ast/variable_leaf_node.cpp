#include "compiler/ast/variable_leaf_node.h"
#include "compiler/ast/AST.h"
variable_leaf_node::variable_leaf_node(symbol::ptr_type _symPtr):symPtr(_symPtr) {

}

bool variable_leaf_node::evaluatable() const {
  return false;
}

double variable_leaf_node::evaluate() const {
  return static_cast<double>(0);
}

std::ostream& variable_leaf_node::print(std::ostream& out) const {
  out << symPtr->name;
  return out;
}

std::ostream& variable_leaf_node::emit_code_ia32(std::ostream& acc, compiler_data& data) const {
  if(data.stackSizeFPU >= 8) {
    acc << "fdecstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF6';

    /*Make room for the extended double precision data on the stack*/
    acc << "subl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xEC';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fstpt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x3C';
    data.executableBuf[++data.offset] = '\x24';
  }

  /*Increment the size of the stack to reflected the added value*/
  ++data.stackSizeFPU;


  unsigned int offset = 8 * (symPtr->parameter + 1);
  acc << "fldl " <<  offset << "(%ebp)\n";
  data.executableBuf[++data.offset] = '\xDD';
  data.executableBuf[++data.offset] = '\x85';
  data.executableBuf[++data.offset] = (offset) & 0xFF;
  data.executableBuf[++data.offset] = (offset >> 8) & 0xFF;
  data.executableBuf[++data.offset] = (offset >> 16) & 0xFF;
  data.executableBuf[++data.offset] = (offset >> 24) & 0xFF;
  return acc;
}

std::ostream& variable_leaf_node::emit_code_amd64(std::ostream& acc, compiler_data& data) const {
  AST::emit_stack_inc_amd64(acc, data);
  unsigned int offset;
  if(symPtr->parameter <= 7) {
    offset = reinterpret_cast<unsigned int>(-72 + symPtr->parameter*8);
  } else {
    offset = 16 + 8 * (symPtr->parameter - 8);
  }

  acc << "fldl " <<  offset << "(%rbp)\n";
  data.executableBuf[++data.offset] = '\xDD';
  data.executableBuf[++data.offset] = '\x85';
  data.executableBuf[++data.offset] = (offset) & 0xFF;
  data.executableBuf[++data.offset] = (offset >> 8) & 0xFF;
  data.executableBuf[++data.offset] = (offset >> 16) & 0xFF;
  data.executableBuf[++data.offset] = (offset >> 24) & 0xFF;
  return acc;
}

unsigned int variable_leaf_node::code_size() const {
  return 14;
}

expression_node* variable_leaf_node::copy() const {
  return AST::make_variable_leaf_node(symPtr);
}

bool variable_leaf_node::is_integral() const {
  return false;
}
