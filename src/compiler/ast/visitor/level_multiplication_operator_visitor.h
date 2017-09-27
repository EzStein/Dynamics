/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   level_multiplication_operator_visitor.h
 * Author: ezra
 *
 * Created on September 26, 2017, 8:13 PM
 */

#ifndef LEVEL_MULTIPLICATION_OPERATOR_VISITOR_H
#define LEVEL_MULTIPLICATION_OPERATOR_VISITOR_H
#include "visitor.h"
#include <list>

class level_multiplication_operator_visitor : public visitor {
public:
  level_multiplication_operator_visitor(std::list<expression_node*>& _newChildren);
  /*
   * If we visit a multiplication node, we add the children
   * of the multiplication node to the newChildren list.
   * We also need to safely delete the multiplication
   * node since it will no longer be referenced
   */
  void visit(polyadic_multiplication_operator_node* node) override;

  /**
   * Otherwise we add the node itself to the list.
   */
  void visit(binary_addition_operator_node* node) override;

  void visit(binary_subtraction_operator_node* node) override;

  void visit(division_operator_node* node) override;

  void visit(exponentiation_operator_node* node) override;

  void visit(polyadic_addition_operator_node* node) override;

  void visit(binary_multiplication_operator_node* node) override;

  void visit(unary_minus_operator_node* node) override;

  void visit(integer_number_leaf_node* node) override;

  void visit(number_leaf_node* node) override;

  void visit(variable_leaf_node* node) override;

private:
  std::list<expression_node*>& newChildren;
};

#endif /* LEVEL_MULTIPLICATION_OPERATOR_VISITOR_H */
