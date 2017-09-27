/*
 *Identifies a polyadic_addition_operator_node by
 * setting the provided reference to point to it.
 * Otherwise the reference remains unchanged.
 */

#include "pre_canonical_multiplication_operator_identifier_visitor.h"
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

pre_canonical_multiplication_operator_identifier_visitor::pre_canonical_multiplication_operator_identifier_visitor(
  polyadic_addition_operator_node*& _result) : result(_result) { }


void pre_canonical_multiplication_operator_identifier_visitor::visit(polyadic_multiplication_operator_node* node) {
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(integer_number_leaf_node* node) {
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(number_leaf_node* node) {
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(variable_leaf_node* node) {
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(exponentiation_operator_node* node) {
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(polyadic_addition_operator_node* node) {
  result = node;
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(binary_multiplication_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(unary_minus_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(binary_addition_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(binary_subtraction_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_multiplication_operator_identifier_visitor::visit(division_operator_node* node) {
  throw "Cannot visit this node";
}