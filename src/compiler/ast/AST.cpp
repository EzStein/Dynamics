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

std::ostream& AST::emit_code(std::ostream& acc) const {
  acc << "pushl %ebp\n";
  acc << "movl %esp, %ebp\n";
  acc << "subl $6, %esp\n";
  acc << "fstcw -2(%ebp)\n";
  acc << "stmxcsr -6(%ebp)\n";
  acc << "pushl %ebx\n";
  acc << "pushl %esi\n";
  acc << "pushl %ebi\n";
  acc << "finit\n";
  root->emit_code(acc);
  acc << "popl %ebi\n";
  acc << "popl %esi\n";
  acc << "popl %ebx\n";
  acc << "movl %ebp, %esp\n";
  acc << "popl %ebp\n";
  acc << "ret\n";
  return acc;
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
