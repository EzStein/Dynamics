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

#ifndef POLYADIC_ADDITION_EQUALITY_VISITOR_H
#define POLYADIC_ADDITION_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
class polyadic_addition_equality_visitor : public cascaded_equality_base_visitor {
private:
  polyadic_addition_operator_node* node1;
public:
  polyadic_addition_equality_visitor(polyadic_addition_operator_node* node1,bool& retVal);
  void visit(polyadic_addition_operator_node* node) override;
};


#endif /* POLYADIC_ADDITION_EQUALITY_VISITOR_H */
