/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "exponentiation_equality_visitor.h"

exponentiation_equality_visitor::exponentiation_equality_visitor(
exponentiation_operator_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}


void exponentiation_equality_visitor::visit(exponentiation_operator_node* node2) {
  retVal = *(node1->leftChild) == *(node2->leftChild) && *(node1->rightChild) == *(node2->rightChild);
}
