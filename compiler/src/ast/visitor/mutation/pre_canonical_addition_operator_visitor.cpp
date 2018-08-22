/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "compiler/ast/visitor/mutation/pre_canonical_addition_operator_visitor.h"
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

pre_canonical_addition_operator_visitor::pre_canonical_addition_operator_visitor(
  std::list<expression_node*>& _newChildren,std::list<expression_node*>& _toDelete)
: newChildren(_newChildren), toDelete(_toDelete) { }

/*
 * In this case we simply add the node as a child
 * because polyadic addition nodes can have polyadic multiplication node children
 */
void pre_canonical_addition_operator_visitor::visit(polyadic_multiplication_operator_node* node) {
  newChildren.push_back(node);
}

/*
 * Here we form the necessary additions.
 */
void pre_canonical_addition_operator_visitor::visit(integer_number_leaf_node* node) {
  expression_node* newNode =
    new polyadic_multiplication_operator_node(
    new exponentiation_operator_node(
    node, new integer_number_leaf_node(1)));
  newChildren.push_back(newNode);
}

void pre_canonical_addition_operator_visitor::visit(number_leaf_node* node) {
  expression_node* newNode =
    new polyadic_multiplication_operator_node(
    new exponentiation_operator_node(
    node, new integer_number_leaf_node(1)));
  newChildren.push_back(newNode);
}

void pre_canonical_addition_operator_visitor::visit(variable_leaf_node* node) {
  expression_node* newNode =
    new polyadic_multiplication_operator_node(
    new exponentiation_operator_node(
    node, new integer_number_leaf_node(1)));
  newChildren.push_back(newNode);
}

void pre_canonical_addition_operator_visitor::visit(exponentiation_operator_node* node) {
  expression_node* newNode =
    new polyadic_multiplication_operator_node(node);
  newChildren.push_back(newNode);
}

/*
 * Here we level operators like normal
 */
void pre_canonical_addition_operator_visitor::visit(polyadic_addition_operator_node* node) {
  std::list<expression_node*>::const_iterator iter = node->children.begin();
  std::list<expression_node*>::const_iterator end = node->children.end();
  for(; iter != end; ++iter) {
    newChildren.push_back(*iter);
  }
  /*We are now going to delete the node, so we clear its children*/
  node->children.clear();
  toDelete.push_back(node);
}

void pre_canonical_addition_operator_visitor::visit(binary_multiplication_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_addition_operator_visitor::visit(unary_minus_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_addition_operator_visitor::visit(binary_addition_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_addition_operator_visitor::visit(binary_subtraction_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_addition_operator_visitor::visit(division_operator_node* node) {
  throw "Cannot visit this node";
}

