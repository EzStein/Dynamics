/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   binary_addition_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef BINARY_ADDITION_EQUALITY_VISITOR_H
#define BINARY_ADDITION_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/binary_addition_operator_node.h"
class binary_addition_equality_visitor : public cascaded_equality_base_visitor {
private:
  binary_addition_operator_node* node1;
public:
  binary_addition_equality_visitor(binary_addition_operator_node* node1,bool& retVal);
  void visit(binary_addition_operator_node* node) override;
};


#endif /* binary_ADDITION_EQUALITY_VISITOR_H */
