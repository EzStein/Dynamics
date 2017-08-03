#include <new>
#include "AST.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/plus_operator_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/binary_minus_operator_node.h"
#include "compiler/ast/multiply_operator_node.h"
#include "compiler/ast/divide_operator_node.h"
#include "compiler/ast/factorial_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/compiler_data.h"

AST::AST() : AST(1000) { }

AST::AST(size_t size) : root(nullptr), allocatorOffset(0), memSize(size) {
  mem = new unsigned char[memSize];
}

AST::~AST() {
  delete[] mem;
}

expression_node* AST::make_variable_leaf_node(variable_leaf_node::symbol_ptr_type ptr) {
  expression_node* ret = new (mem + allocatorOffset) variable_leaf_node(ptr);
  allocatorOffset += sizeof(variable_leaf_node);
  return ret;
}

expression_node* AST::make_number_leaf_node(double val) {
  expression_node* ret = new (mem + allocatorOffset) number_leaf_node(val);
  allocatorOffset += sizeof(number_leaf_node);
  return ret;
}

double AST::evaluate() const {
  return root->evaluate();
}

void AST::set_root(expression_node* node) {
  root = node;
}

std::ostream& AST::emit_code(std::ostream& acc, unsigned char * buf) const {
  compiler_data data{-1, 0, buf};
  acc << "pushl %ebp\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF5';

  acc << "movl %esp, %ebp\n";
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\xE5';

  acc << "subl $8, %esp\n";
  data.executableBuf[++data.offset] = '\x81';
  data.executableBuf[++data.offset] = '\xEC';
  data.executableBuf[++data.offset] = '\x08';
  data.executableBuf[++data.offset] = '\x00';
  data.executableBuf[++data.offset] = '\x00';
  data.executableBuf[++data.offset] = '\x00';

  acc << "fstcw -2(%ebp)\n";
  data.executableBuf[++data.offset] = '\x9B';
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\x7D';
  data.executableBuf[++data.offset] = '\xFE';

  acc << "stmxcsr -6(%ebp)\n";
  data.executableBuf[++data.offset] = '\x0F';
  data.executableBuf[++data.offset] = '\xAE';
  data.executableBuf[++data.offset] = '\x5D';
  data.executableBuf[++data.offset] = '\xFA';

  acc << "movw $0x0f7e, -8(%ebp)\n";
  data.executableBuf[++data.offset] = '\x66';
  data.executableBuf[++data.offset] = '\xC7';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xF8';
  data.executableBuf[++data.offset] = '\x7E';
  data.executableBuf[++data.offset] = '\x0F';

  acc << "pushl %ebx\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF3';

  acc << "pushl %esi\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF6';

  acc << "pushl %edi\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF7';

  acc << "finit\n";
  data.executableBuf[++data.offset] = '\x9B';
  data.executableBuf[++data.offset] = '\xDB';
  data.executableBuf[++data.offset] = '\xE3';

  acc << "fldcw -8(%ebp)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\x6D';
  data.executableBuf[++data.offset] = '\xF8';

  root->emit_code(acc, data);

  acc << "popl %edi\n";
  data.executableBuf[++data.offset] = '\x5F';

  acc << "popl %esi\n";
  data.executableBuf[++data.offset] = '\x5E';

  acc << "popl %ebx\n";
  data.executableBuf[++data.offset] = '\x5B';

  acc << "fldcw -2(%ebp)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\x6D';
  data.executableBuf[++data.offset] = '\xFE';

  acc << "ldmxcsr -6(%ebp)\n";
  data.executableBuf[++data.offset] = '\x0F';
  data.executableBuf[++data.offset] = '\xAE';
  data.executableBuf[++data.offset] = '\x55';
  data.executableBuf[++data.offset] = '\xFA';

  acc << "movl %ebp, %esp\n";
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\xEC';

  acc << "popl %ebp\n";
  data.executableBuf[++data.offset] = '\x5D';

  acc << "ret\n";
  data.executableBuf[++data.offset] = '\xC3';

  return acc;
}

unsigned int AST::code_size() const {
  return root->code_size() + 50;
}

template<class NODE_TYPE>
expression_node* AST::make_binary_operator_node(expression_node* leftChild, expression_node* rightChild) {
  expression_node* ret = new (mem + allocatorOffset) NODE_TYPE(leftChild, rightChild);
  allocatorOffset += sizeof(NODE_TYPE);
  return ret;
}

template<class NODE_TYPE>
expression_node* AST::make_unary_operator_node(expression_node* child) {
  expression_node* ret = new (mem + allocatorOffset) NODE_TYPE(child);
  allocatorOffset += sizeof(NODE_TYPE);
  return ret;
}

template expression_node* AST::make_binary_operator_node<exponentiation_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<plus_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<binary_minus_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<multiply_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<divide_operator_node>(expression_node*,expression_node*);

template expression_node* AST::make_unary_operator_node<factorial_operator_node>(expression_node*);
template expression_node* AST::make_unary_operator_node<unary_minus_operator_node>(expression_node*);

std::ostream& operator<<(std::ostream& out, const AST& ast) {
  ast.root->print(out);
  return out;
}
