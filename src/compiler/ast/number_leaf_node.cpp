#include <iomanip>
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/AST.h"

number_leaf_node::number_leaf_node(double _val) : val(_val) {

}

double number_leaf_node::evaluate() const {
  return val;
}

std::ostream& number_leaf_node::print(std::ostream& out) const {
  out << val;
  return out;
}

bool number_leaf_node::evaluatable() const {
  return true;
}

std::ostream& number_leaf_node::emit_code_ia32(std::ostream& acc, compiler_data& data) const {
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

  const unsigned char * ptr = reinterpret_cast<const unsigned char*>(&val);
  acc << std::hex;
  acc << "pushl $0x";
  for(int i = 7; i >= 4; --i) {
    const unsigned int toWrite = static_cast<const unsigned int>(ptr[i]);
    if(toWrite <= 0x0f)
      acc << "0";
    acc << toWrite;
  }

  data.executableBuf[++data.offset] = '\x68';
  for(int i = 4; i <= 7; ++i)
    data.executableBuf[++data.offset] = ptr[i];

  acc << "\n";
  acc << "pushl $0x";
  for(int i = 3; i >= 0; --i) {
    const unsigned int toWrite = static_cast<const unsigned int>(ptr[i]);
    if(toWrite <= 0x0f)
      acc << "0";
    acc << toWrite;
  }
  acc << "\n";
  acc << std::dec;
  data.executableBuf[++data.offset] = '\x68';
  for(int i = 0; i <= 3; ++i)
    data.executableBuf[++data.offset] = ptr[i];

  acc << "fldl (%esp)\n";
  data.executableBuf[++data.offset] = '\xDD';
  data.executableBuf[++data.offset] = '\x04';
  data.executableBuf[++data.offset] = '\x24';

  acc << "addl $8, %esp\n";
  data.executableBuf[++data.offset] = '\x81';
  data.executableBuf[++data.offset] = '\xC4';
  data.executableBuf[++data.offset] = '\x08';
  data.executableBuf[++data.offset] = '\x00';
  data.executableBuf[++data.offset] = '\x00';
  data.executableBuf[++data.offset] = '\x00';

  return acc;
}

std::ostream& number_leaf_node::emit_code_amd64(std::ostream& acc, compiler_data& data) const {
  AST::emit_stack_inc_amd64(acc, data);

  const unsigned char * ptr = reinterpret_cast<const unsigned char*>(&val);
  acc << std::hex;
  acc << "movq $0x";
  for(int i = 7; i >= 0; --i) {
    const unsigned int toWrite = static_cast<const unsigned int>(ptr[i]);
    if(toWrite <= 0x0f)
      acc << "0";
    acc << toWrite;
  }
  acc << ", %rax\n";

  data.executableBuf[++data.offset] = '\x48';
  data.executableBuf[++data.offset] = '\xb8';
  for(int i = 0; i <= 7; ++i)
    data.executableBuf[++data.offset] = ptr[i];

  acc << "movq %rax, -0x50(%rbp)\n";
  data.executableBuf[++data.offset] = '\x48';
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xb0';

  acc << "fldl -0x50(%rbp)\n";
  data.executableBuf[++data.offset] = '\xdd';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xb0';

  return acc;
}

unsigned int number_leaf_node::code_size() const {
  return 30;
}

expression_node* number_leaf_node::copy() const {
  return AST::make_number_leaf_node(val);
}

bool number_leaf_node::is_integral() const {
  return false;
}

void number_leaf_node::accept(visitor& v) {
  v.visit(this);
}

