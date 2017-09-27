/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "level_multiplication_operator_visitor.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"

level_multiplication_operator_visitor::level_multiplication_operator_visitor(
  std::list<expression_node*>& _newChildren,std::list<expression_node*>& _toDelete)
: newChildren(_newChildren), toDelete(_toDelete) { }

/*
 * If we level_multiplication_operator_visitor::visit a multiplication node, we add the children
 * of the multiplication node to the newChildren list.
 * We also need to safely delete the multiplication
 * node since it will no longer be referenced
 */
void level_multiplication_operator_visitor::visit(polyadic_multiplication_operator_node* node) {
  std::list<expression_node*>::const_iterator iter = node->children.begin();
  std::list<expression_node*>::const_iterator end = node->children.end();
  for(; iter != end; ++iter) {
    newChildren.push_back(*iter);
  }
  /*We are now going to delete the node, so we clear its children*/
  node->children.clear();
  toDelete.push_back(node);
}

/**
 * Otherwise we add the node itself to the list.
 */
void level_multiplication_operator_visitor::visit(binary_addition_operator_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(binary_subtraction_operator_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(division_operator_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(exponentiation_operator_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(polyadic_addition_operator_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(binary_multiplication_operator_node* node) {
  newChildren.push_back(node->leftChild);
  newChildren.push_back(node->rightChild);
  node->leftChild = nullptr;
  node->rightChild = nullptr;
  toDelete.push_back(node);
}

void level_multiplication_operator_visitor::visit(unary_minus_operator_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(integer_number_leaf_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(number_leaf_node* node) {
  newChildren.push_back(node);
}

void level_multiplication_operator_visitor::visit(variable_leaf_node* node) {
  newChildren.push_back(node);
}
