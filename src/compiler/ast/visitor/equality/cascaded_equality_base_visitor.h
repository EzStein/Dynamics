/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   cascaded_equality_base_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:18 PM
 *
 * This class is the base of all second tier equality checks.
 * In any given method of the class the dynamic type is known
 * of both parameters. This class
 * just sets the return value to false in each case and subclasses
 * override this default behavior if they want.
 */

#ifndef CASCADED_EQUALITY_BASE_VISITOR_H
#define CASCADED_EQUALITY_BASE_VISITOR_H
#include "compiler/ast/visitor/visitor.h"
class cascaded_equality_base_visitor : public visitor {
protected:
  bool& retVal;

public:
  cascaded_equality_base_visitor(bool&);

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

#endif /* CASCADED_EQUALITY_BASE_VISITOR_H */
