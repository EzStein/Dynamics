/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dual_parameter_visitor.h
 * Author: ezra
 *
 * This is an interface class used to implement dual parameter double dispatch.
 * In general if there are n node types this class will have n^2 methods which is a
 * lot. Essentially it is used to implement things like
 * equality and comparisons between two nodes which
 * require a lot of logic.
 */

#ifndef DUAL_PARAMETER_VISITOR_H
#define DUAL_PARAMETER_VISITOR_H
class expression_node;
class binary_addition_operator_node;
class binary_subtraction_operator_node;
class binary_multiplication_operator_node;
class unary_minus_operator_node;
class exponentiation_operator_node;
class division_operator_node;
class polyadic_addition_operator_node;
class polyadic_multiplication_operator_node;
class integer_number_leaf_node;
class number_leaf_node;
class variable_leaf_node;

class dual_parameter_visitor {
public:
  void dispatch(expression_node* node1, expression_node* node2);

  virtual void visit(binary_addition_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(binary_addition_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(binary_subtraction_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(binary_subtraction_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(binary_multiplication_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(binary_multiplication_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(unary_minus_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(unary_minus_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(exponentiation_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(exponentiation_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(division_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(division_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(division_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(division_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(polyadic_addition_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(polyadic_addition_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(polyadic_multiplication_operator_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, division_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(polyadic_multiplication_operator_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(integer_number_leaf_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, division_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(integer_number_leaf_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(number_leaf_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, division_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(number_leaf_node* node1, variable_leaf_node* node2) = 0;

  virtual void visit(variable_leaf_node* node1, binary_addition_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, binary_subtraction_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, binary_multiplication_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, unary_minus_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, exponentiation_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, division_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, polyadic_addition_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, polyadic_multiplication_operator_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, integer_number_leaf_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, number_leaf_node* node2) = 0;
  virtual void visit(variable_leaf_node* node1, variable_leaf_node* node2) = 0;
};

#endif /* DUAL_PARAMETER_VISITOR_H */
