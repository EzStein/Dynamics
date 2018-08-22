/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "compiler/ast/visitor/dual/dual_visitor.h"
#include "compiler/ast/visitor/dual/polyadic_addition_dual_visitor.h"
#include "compiler/ast/visitor/dual/polyadic_multiplication_dual_visitor.h"
#include "compiler/ast/visitor/dual/binary_addition_dual_visitor.h"
#include "compiler/ast/visitor/dual/binary_multiplication_dual_visitor.h"
#include "compiler/ast/visitor/dual/binary_subtraction_dual_visitor.h"
#include "compiler/ast/visitor/dual/unary_minus_dual_visitor.h"
#include "compiler/ast/visitor/dual/integer_number_dual_visitor.h"
#include "compiler/ast/visitor/dual/number_dual_visitor.h"
#include "compiler/ast/visitor/dual/variable_dual_visitor.h"
#include "compiler/ast/visitor/dual/exponentiation_dual_visitor.h"
#include "compiler/ast/visitor/dual/division_dual_visitor.h"

dual_visitor::dual_visitor(expression_node* _node2, dual_parameter_visitor* _callBackVisitor)
: node2(_node2), callBackVisitor(_callBackVisitor) {

}

void dual_visitor::visit(polyadic_addition_operator_node* node1) {
  polyadic_addition_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(binary_addition_operator_node* node1) {
  binary_addition_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(polyadic_multiplication_operator_node* node1) {
  polyadic_multiplication_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(binary_multiplication_operator_node* node1) {
  binary_multiplication_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(binary_subtraction_operator_node* node1) {
  binary_subtraction_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(unary_minus_operator_node* node1) {
  unary_minus_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(integer_number_leaf_node* node1) {
  integer_number_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(number_leaf_node* node1) {
  number_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(variable_leaf_node* node1) {
  variable_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(exponentiation_operator_node* node1) {
  exponentiation_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}

void dual_visitor::visit(division_operator_node* node1) {
  division_dual_visitor visit(node1, callBackVisitor);
  node2->accept(&visit);
}
