/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   integer_number_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef integer_number_EQUALITY_VISITOR_H
#define integer_number_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/integer_number_leaf_node.h"
class integer_number_equality_visitor : public cascaded_equality_base_visitor {
private:
  integer_number_leaf_node* node1;
public:
  integer_number_equality_visitor(integer_number_leaf_node* node1,bool& retVal);
  void visit(integer_number_leaf_node* node) override;
};


#endif /* integer_number_EQUALITY_VISITOR_H */
