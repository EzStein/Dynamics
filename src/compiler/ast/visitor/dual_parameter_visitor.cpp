/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "compiler/ast/visitor/dual_parameter_visitor.h"
#include "compiler/ast/visitor/dual/dual_visitor.h"
void dual_parameter_visitor::dispatch(expression_node* node1, expression_node* node2) {
  dual_visitor visit(node2, this);
  visit->accept(node1);
}
