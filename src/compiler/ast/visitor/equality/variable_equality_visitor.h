/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   variable_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef variable_EQUALITY_VISITOR_H
#define variable_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/variable_leaf_node.h"
class variable_equality_visitor : public cascaded_equality_base_visitor {
private:
  variable_leaf_node* node1;
public:
  variable_equality_visitor(variable_leaf_node* node1,bool& retVal);
  void visit(variable_leaf_node* node) override;
};


#endif /* variable_EQUALITY_VISITOR_H */
