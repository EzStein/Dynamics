/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   unary_minus_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef unary_minus_EQUALITY_VISITOR_H
#define unary_minus_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/unary_minus_operator_node.h"
class unary_minus_equality_visitor : public cascaded_equality_base_visitor {
private:
  unary_minus_operator_node* node1;
public:
  unary_minus_equality_visitor(unary_minus_operator_node* node1,bool& retVal);
  void visit(unary_minus_operator_node* node) override;
};


#endif /* unary_minus_EQUALITY_VISITOR_H */
