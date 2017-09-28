/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:17 PM
 * 
 * This visitor is used to implement equality testing between two elements in the tree.
 * It is constructed with an expression_node to compare to, and a reference to a bool.
 * When it visits a node it uses a second visitor derived from
 * cascaded_equality_base_visitor on the node to determine whether the two nodes are equal.
 * In this way double dispatch is used to determine the type of two parameters.
 */

#ifndef EQUALITY_VISITOR_H
#define EQUALITY_VISITOR_H
#include "compiler/ast/visitor/visitor.h"
class equality_visitor : public visitor {
private:
  expression_node* nodeToCompare;
  bool& retVal;
public:
  equality_visitor(expression_node*, bool&);
  
  void visit(polyadic_addition_operator_node* node) override;

  void visit(polyadic_multiplication_operator_node* node) override;

  void visit(binary_subtraction_operator_node* node) override;

  void visit(division_operator_node* node) override;

  void visit(exponentiation_operator_node* node) override;

  void visit(binary_addition_operator_node* node) override;

  void visit(binary_multiplication_operator_node* node) override;

  void visit(unary_minus_operator_node* node) override;

  void visit(integer_number_leaf_node* node) override;

  void visit(number_leaf_node* node) override;

  void visit(variable_leaf_node* node) override;
};



#endif /* EQUALITY_VISITOR_H */

