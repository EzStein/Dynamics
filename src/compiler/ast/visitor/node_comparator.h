/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   node_comparator.h
 * Author: ezra
 *
 * Created on September 28, 2017, 5:41 PM
 * Implements the dual_parameter_visitor
 * to compare nodes. When
 * dispatch(node1, node2) is called,
 * the provided reference is given a value
 * of -1 if node1 < node2
 * 0 if node1 == node2
 * 1 if node1 > node2
 *
 *
 * In general a node A is less then node B if
 * A has fewer children the B. If this is the case,
 * B > A as well. If the number of children
 * are the same then an ordering is chosen arbitrarily
 * to allow for useful pre-canonical form term collections.
 * A = B if they are the same node type and their children are equal.
 *
 * When comparing nodes with the same number of
 * children but have different types
 * - < + < / < * < ^
 *
 * When comparing two nodes of the same operator type but that are not the
 * same type such as a binary and a polyadic operator and both nodes have
 * the same number of children, the polyadic operator will be considered greater.
 *
 * Note, if two nodes are the same operator type
 * and have the same number of children,
 * we compare each child pairwise and the
 * first descrepency is reported.
 * If all the children are compared as equal then
 * the nodes are considered equal.
 */

#ifndef NODE_COMPARATOR_H
#define NODE_COMPARATOR_H
#include "compiler/ast/visitor/dual_parameter_visitor.h"
class node_comparator : public dual_parameter_visitor {
public:
  int& retVal;
  node_comparator(int& _retVal) : retVal(_retVal) { }

  virtual void visit(binary_addition_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(binary_addition_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(binary_subtraction_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(binary_subtraction_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(binary_multiplication_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(binary_multiplication_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(unary_minus_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(unary_minus_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(exponentiation_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(exponentiation_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(division_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(division_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(division_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(division_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(polyadic_addition_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(polyadic_addition_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(polyadic_multiplication_operator_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, division_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, number_leaf_node* node2) override;
  virtual void visit(polyadic_multiplication_operator_node* node1, variable_leaf_node* node2) override;

  virtual void visit(integer_number_leaf_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, division_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, number_leaf_node* node2) override;
  virtual void visit(integer_number_leaf_node* node1, variable_leaf_node* node2) override;

  virtual void visit(number_leaf_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, division_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(number_leaf_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(number_leaf_node* node1, number_leaf_node* node2) override;
  virtual void visit(number_leaf_node* node1, variable_leaf_node* node2) override;

  virtual void visit(variable_leaf_node* node1, binary_addition_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, binary_subtraction_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, binary_multiplication_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, unary_minus_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, exponentiation_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, division_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, polyadic_addition_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, polyadic_multiplication_operator_node* node2) override;
  virtual void visit(variable_leaf_node* node1, integer_number_leaf_node* node2) override;
  virtual void visit(variable_leaf_node* node1, number_leaf_node* node2) override;
  virtual void visit(variable_leaf_node* node1, variable_leaf_node* node2) override;
};


#endif /* NODE_COMPARATOR_H */
