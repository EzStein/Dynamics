/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "cascaded_equality_base_visitor.h"
#include "compiler/ast/AST.h"

cascaded_equality_base_visitor::cascaded_equality_base_visitor(bool& _retVal) : retVal(_retVal) {

}

void cascaded_equality_base_visitor::visit(polyadic_addition_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(binary_addition_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(polyadic_multiplication_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(binary_multiplication_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(binary_subtraction_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(unary_minus_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(integer_number_leaf_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(number_leaf_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(variable_leaf_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(exponentiation_operator_node* node) {
  retVal = false;
}

void cascaded_equality_base_visitor::visit(division_operator_node* node) {
  retVal = false;
}