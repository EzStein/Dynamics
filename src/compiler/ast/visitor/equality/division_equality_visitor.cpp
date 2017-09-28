/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "division_equality_visitor.h"

division_equality_visitor::division_equality_visitor(
division_operator_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}


void division_equality_visitor::visit(division_operator_node* node2) {
  retVal = *(node1->leftChild) == *(node2->leftChild) && *(node1->rightChild) == *(node2->rightChild);
}
