/*
 * Implements the integer number leaf node
 */
#include "compiler/ast/AST.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include <cmath>

integer_number_leaf_node::integer_number_leaf_node(long _val) : val(_val) { }
integer_number_leaf_node::integer_number_leaf_node(int _val) : val(_val) { }

integer_number_leaf_node::integer_number_leaf_node(double _val) {
  val = std::round(_val);
}

std::ostream& integer_number_leaf_node::emit_code_ia32(std::ostream& out, compiler_data&) const {
  return out;
}

std::ostream& integer_number_leaf_node::emit_code_amd64(std::ostream& acc, compiler_data& data) const {
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

double integer_number_leaf_node::evaluate() const {
  return static_cast<double>(val);
}

/*True if there are no variables in the sub tree whose root is this node*/
bool integer_number_leaf_node::evaluatable() const {
  return true;
}

std::ostream& integer_number_leaf_node::print(std::ostream& out) const {
  out << val;
  return out;
}

/*
* Copies the tree whose root is this node and returns a pointer to this copy
*/
expression_node* integer_number_leaf_node::copy() const {
  return new integer_number_leaf_node(val);
}

/*
* Returns the maximum code size of the produced code.
*/
unsigned int integer_number_leaf_node::code_size() const {
  return 30;
}

bool integer_number_leaf_node::is_integral() const {
  return true;
}

void integer_number_leaf_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* integer_number_leaf_node::differentiate(const std::string& var) {
  return new integer_number_leaf_node(0);
}
