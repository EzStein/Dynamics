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
  unsigned int offset = 0;
  acc << "pushl %ebp\n";
  buf[offset] = '\xFF';
  buf[++offset] = '\xF5';

  acc << "movl %esp, %ebp\n";
  buf[++offset] = '\x89';
  buf[++offset] = '\xE5';

  acc << "subl $8, %esp\n";
  buf[++offset] = '\x81';
  buf[++offset] = '\xEC';
  buf[++offset] = '\x08';
  buf[++offset] = '\x00';
  buf[++offset] = '\x00';
  buf[++offset] = '\x00';

  acc << "fstcw -2(%ebp)\n";
  buf[++offset] = '\x9B';
  buf[++offset] = '\xD9';
  buf[++offset] = '\x7D';
  buf[++offset] = '\xFE';

  acc << "stmxcsr -6(%ebp)\n";
  buf[++offset] = '\x0F';
  buf[++offset] = '\xAE';
  buf[++offset] = '\x5D';
  buf[++offset] = '\xFA';

  acc << "movw $0x0f7f, -8(%ebp)\n";
  buf[++offset] = '\x66';
  buf[++offset] = '\xC7';
  buf[++offset] = '\x45';
  buf[++offset] = '\xF8';
  buf[++offset] = '\x7F';
  buf[++offset] = '\x0F';

  acc << "pushl %ebx\n";
  buf[++offset] = '\xFF';
  buf[++offset] = '\xF3';

  acc << "pushl %esi\n";
  buf[++offset] = '\xFF';
  buf[++offset] = '\xF6';

  acc << "pushl %edi\n";
  buf[++offset] = '\xFF';
  buf[++offset] = '\xF7';

  acc << "finit\n";
  buf[++offset] = '\x9B';
  buf[++offset] = '\xDB';
  buf[++offset] = '\xE3';

  acc << "fldcw -8(%ebp)\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\x6D';
  buf[++offset] = '\xF8';
  ++offset;

  root->emit_code(acc, buf, offset);


  acc << "popl %edi\n";
  buf[offset] = '\x5F';

  acc << "popl %esi\n";
  buf[++offset] = '\x5E';

  acc << "popl %ebx\n";
  buf[++offset] = '\x5B';

  acc << "fldcw -2(%ebp)\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\x6D';
  buf[++offset] = '\xFE';

  acc << "ldmxcsr -6(%ebp)\n";
  buf[++offset] = '\x0F';
  buf[++offset] = '\xAE';
  buf[++offset] = '\x55';
  buf[++offset] = '\xFA';

  acc << "movl %ebp, %esp\n";
  buf[++offset] = '\x89';
  buf[++offset] = '\xEC';

  acc << "popl %ebp\n";
  buf[++offset] = '\x5D';

  acc << "ret\n";
  buf[++offset] = '\xC3';

  ++offset;
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
