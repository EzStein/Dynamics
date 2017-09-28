/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   number_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef number_EQUALITY_VISITOR_H
#define number_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/number_leaf_node.h"
class number_equality_visitor : public cascaded_equality_base_visitor {
private:
  number_leaf_node* node1;
public:
  number_equality_visitor(number_leaf_node* node1,bool& retVal);
  void visit(number_leaf_node* node) override;
};


#endif /* number_EQUALITY_VISITOR_H */
