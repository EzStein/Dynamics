/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "unary_minus_equality_visitor.h"
#include "compiler/ast/binary_operator_node.h"

unary_minus_equality_visitor::unary_minus_equality_visitor(
unary_minus_operator_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}


void unary_minus_equality_visitor::visit(unary_minus_operator_node* node2) {
  retVal = *(node1->child) == *(node2->child);
}
