/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "integer_number_equality_visitor.h"

integer_number_equality_visitor::integer_number_equality_visitor(
integer_number_leaf_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}

void integer_number_equality_visitor::visit(integer_number_leaf_node* node2) {
  retVal = node1->val == node2->val;
}
