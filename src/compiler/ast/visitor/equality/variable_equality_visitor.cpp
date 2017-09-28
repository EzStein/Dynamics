/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "variable_equality_visitor.h"

variable_equality_visitor::variable_equality_visitor(
variable_leaf_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}

void variable_equality_visitor::visit(variable_leaf_node* node2) {
  retVal = *(node1->symPtr) == *(node2->symPtr);
}
