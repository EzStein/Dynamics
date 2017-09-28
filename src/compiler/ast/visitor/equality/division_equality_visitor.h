/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   division_equality_visitor.h
 * Author: ezra
 *
 * Created on September 27, 2017, 5:34 PM
 */

#ifndef division_EQUALITY_VISITOR_H
#define division_EQUALITY_VISITOR_H
#include "compiler/ast/visitor/equality/cascaded_equality_base_visitor.h"
#include "compiler/ast/division_operator_node.h"
class division_equality_visitor : public cascaded_equality_base_visitor {
private:
  division_operator_node* node1;
public:
  division_equality_visitor(division_operator_node* node1,bool& retVal);
  void visit(division_operator_node* node) override;
};


#endif /* division_EQUALITY_VISITOR_H */
