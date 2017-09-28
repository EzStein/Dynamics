/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "compiler/ast/visitor/dual/variable_dual_visitor.h"
void variable_dual_visitor::visit(binary_addition_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(division_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(polyadic_multiplication_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(unary_minus_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(integer_number_leaf_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(number_leaf_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(polyadic_addition_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(binary_subtraction_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(binary_multiplication_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(exponentiation_operator_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}

void variable_dual_visitor::visit(variable_leaf_node* node2) {
  callBackVisitor->dispatch(node1, node2);
}
