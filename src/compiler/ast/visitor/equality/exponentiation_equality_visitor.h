/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   exponentiation_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef exponentiation_EQUALITY_VISITOR_H
#define exponentiation_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/exponentiation_operator_node.h"
class exponentiation_equality_visitor : public cascaded_equality_base_visitor {
private:
  exponentiation_operator_node* node1;
public:
  exponentiation_equality_visitor(exponentiation_operator_node* node1,bool& retVal);
  void visit(exponentiation_operator_node* node) override;
};


#endif /* exponentiation_EQUALITY_VISITOR_H */
