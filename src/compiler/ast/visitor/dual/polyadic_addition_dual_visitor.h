/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   polyadic_addition_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef polyadic_addition_EQUALITY_VISITOR_H
#define polyadic_addition_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/visitor.h"
#include "compiler/ast/visitor/dual_parameter_visitor.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
class polyadic_addition_dual_visitor : public visitor {
private:
  polyadic_addition_operator_node* node1;
  dual_parameter_visitor* callBackVisitor;
public:
  polyadic_addition_dual_visitor(polyadic_addition_operator_node* _node1,
    dual_parameter_visitor* _callBackVisitor) : node1(_node1), callBackVisitor(_callBackVisitor) { }
  void visit(binary_addition_operator_node* node2) override;
  void visit(division_operator_node* node2) override;
  void visit(polyadic_multiplication_operator_node* node2) override;
  void visit(unary_minus_operator_node* node2) override;
  void visit(integer_number_leaf_node* node2) override;
  void visit(number_leaf_node* node2) override;
  void visit(polyadic_addition_operator_node* node2) override;
  void visit(binary_subtraction_operator_node* node2) override;
  void visit(binary_multiplication_operator_node* node2) override;
  void visit(exponentiation_operator_node* node2) override;
  void visit(variable_leaf_node* node2) override;
};
#endif /* polyadic_addition_EQUALITY_VISITOR_H */
