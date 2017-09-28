/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "binary_addition_equality_visitor.h"
#include "compiler/ast/binary_operator_node.h"

binary_addition_equality_visitor::binary_addition_equality_visitor(
binary_addition_operator_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}


void binary_addition_equality_visitor::visit(binary_addition_operator_node* node2) {
  retVal = *(node1->leftChild) == *(node2->leftChild) && *(node1->rightChild) == *(node2->rightChild);
}
