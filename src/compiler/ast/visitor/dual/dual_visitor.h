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
 * This visitor is used to implement dual parameter dispatch.
 * It is constructed with the second node and the method
 * that is called is dependent on the first node.
 * Each method constructs a second node specific visitor
 * using the first node and the pointer to the dual parameter interface
 * class and lets that visitor visit the second node.
 */

#ifndef EQUALITY_VISITOR_H
#define EQUALITY_VISITOR_H
#include "compiler/ast/visitor/visitor.h"
#include "compiler/ast/visitor/dual_parameter_visitor.h"

class dual_visitor : public visitor {
private:
  expression_node* node2;
  dual_parameter_visitor* callBackVisitor;
public:
  dual_visitor(expression_node* node2, dual_parameter_visitor* callBackVisitor);

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
