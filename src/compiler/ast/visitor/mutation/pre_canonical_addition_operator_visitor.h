/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   pre_canonical_polyadic_addition_visitor.h
 * Author: ezra
 *
 * Created on September 26, 2017, 8:13 PM
 */

#ifndef PRE_CANONICAL_ADDITION_OPERATOR_VISITOR_H
#define PRE_CANONICAL_ADDITION_OPERATOR_VISITOR_H
#include "compiler/ast/visitor/visitor.h"
#include <list>

class pre_canonical_addition_operator_visitor : public visitor {
public:
  pre_canonical_addition_operator_visitor(std::list<expression_node*>& _newChildren,std::list<expression_node*>& );
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
  std::list<expression_node*>& newChildren, toDelete;
};

#endif
