/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pre_canonical_multiplication_operator_visitor.h"
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

pre_canonical_multiplication_operator_visitor::pre_canonical_multiplication_operator_visitor(
  std::list<expression_node*>& _newChildren, std::list<expression_node*>& _toDelete)
: newChildren(_newChildren), toDelete(_toDelete) { }

/*
 * In this case we 
 * level the operator like normal.
 */
void pre_canonical_multiplication_operator_visitor::visit(polyadic_multiplication_operator_node* node) {
  std::list<expression_node*>::const_iterator iter = node->children.begin();
  std::list<expression_node*>::const_iterator end = node->children.end();
  for(; iter != end; ++iter) {
    newChildren.push_back(*iter);
  }
  /*We are now going to delete the node, so we clear its children*/
  node->children.clear();
  toDelete.push_back(node);
}

/*
 * Here we form the necessary additions.
 */
void pre_canonical_multiplication_operator_visitor::visit(integer_number_leaf_node* node) {
  expression_node* newNode =
    new exponentiation_operator_node(
    node, new integer_number_leaf_node(1));
  newChildren.push_back(newNode);
}

void pre_canonical_multiplication_operator_visitor::visit(number_leaf_node* node) {
  expression_node* newNode =
    new exponentiation_operator_node(
    node, new integer_number_leaf_node(1));
  newChildren.push_back(newNode);
}

void pre_canonical_multiplication_operator_visitor::visit(variable_leaf_node* node) {
  expression_node* newNode =
    new exponentiation_operator_node(
    node, new integer_number_leaf_node(1));
  newChildren.push_back(newNode);
}

/*
 * Here we can simply push it onto the list immediately
 */
void pre_canonical_multiplication_operator_visitor::visit(exponentiation_operator_node* node) {
  newChildren.push_back(node);
}

/*
 * This is where things get complicated.
 * Here we essentially implement the distributive property. The result is 
 * a polyadic addition node that is not necessarily in pre-canonical form.
 * We put it in pre_canonical form by calling the (recursive) make_pre_canonical method on it.
 * The body of this method only executes if the,
 * resolveDistribution flag is set. If it is set
 * we return a pointer to the resultant addition node as the first element of the
 * children list.
 */
void pre_canonical_multiplication_operator_visitor::visit(polyadic_addition_operator_node* node) {
  if(node->children.size() == 1) {
    /*The node size is equal to one so it is effectively a wrapper for its child
     We add the child to the list by passing this visitor to it*/
    (*node->children.begin())->accept(*this);
    /*We must now delete the addition operator*/
    node->children.clear();
    toDelete.push_back(node);
  } else {
    newChildren.push_back(node);
  }
  /*If the addition operator has multiple children we handle it using distribution later*/
}

void pre_canonical_multiplication_operator_visitor::visit(binary_multiplication_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_visitor::visit(unary_minus_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_visitor::visit(binary_addition_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_visitor::visit(binary_subtraction_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_visitor::visit(division_operator_node* node) {
  throw "Cannot visit this node";
}

