#include "AST.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/plus_operator_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/binary_minus_operator_node.h"
#include "compiler/ast/multiply_operator_node.h"
#include "compiler/ast/divide_operator_node.h"
#include "compiler/ast/factorial_operator_node.h"

AST::AST() : AST(1000) { }

AST::AST(size_t size) : root(nullptr), allocatorOffset(0), memSize(size) {
  mem = new unsigned char[memSize];
}

AST::~AST() {
  delete[] mem;
}

template<class NODE_TYPE>
expression_node* AST::make_binary_operator_node(expression_node* leftChild, expression_node* rightChild) {

}

template<class NODE_TYPE>
expression_node* AST::make_unary_operator_node(expression_node* child) {

}

template expression_node* AST::make_binary_operator_node<exponentiation_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<plus_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<binary_minus_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<multiply_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<divide_operator_node>(expression_node*,expression_node*);

template expression_node* AST::make_unary_operator_node<factorial_operator_node>(expression_node*);
template expression_node* AST::make_unary_operator_node<unary_minus_operator_node>(expression_node*);
