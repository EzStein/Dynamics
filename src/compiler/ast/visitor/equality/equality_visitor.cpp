/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "equality_visitor.h"
#include "compiler/ast/visitor/equality/polyadic_addition_equality_visitor.h"
#include "compiler/ast/visitor/equality/polyadic_multiplication_equality_visitor.h"
#include "compiler/ast/visitor/equality/binary_addition_equality_visitor.h"
#include "compiler/ast/visitor/equality/binary_multiplication_equality_visitor.h"
#include "compiler/ast/visitor/equality/binary_subtraction_equality_visitor.h"

#include "compiler/ast/visitor/equality/unary_minus_equality_visitor.h"
#include "compiler/ast/visitor/equality/integer_number_equality_visitor.h"
#include "compiler/ast/visitor/equality/number_equality_visitor.h"
#include "compiler/ast/visitor/equality/variable_equality_visitor.h"
#include "compiler/ast/visitor/equality/exponentiation_equality_visitor.h"
#include "compiler/ast/visitor/equality/division_equality_visitor.h"

equality_visitor::equality_visitor(expression_node* _nodeToCompare, bool& _retVal)
: nodeToCompare(_nodeToCompare), retVal(_retVal) {

}

void equality_visitor::visit(polyadic_addition_operator_node* node) {
  polyadic_addition_equality_visitor visit(node, retVal);

  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(binary_addition_operator_node* node) {
  binary_addition_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(polyadic_multiplication_operator_node* node) {
  polyadic_multiplication_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(binary_multiplication_operator_node* node) {
  binary_multiplication_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(binary_subtraction_operator_node* node) {
  binary_subtraction_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(unary_minus_operator_node* node) {
  unary_minus_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(integer_number_leaf_node* node) {
  integer_number_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(number_leaf_node* node) {
  number_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(variable_leaf_node* node) {
  variable_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(exponentiation_operator_node* node) {
  exponentiation_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}

void equality_visitor::visit(division_operator_node* node) {
  division_equality_visitor visit(node, retVal);
  nodeToCompare->accept(&visit);
}
