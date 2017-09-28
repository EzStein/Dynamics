/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "polyadic_multiplication_equality_visitor.h"
#include "compiler/ast/polyadic_operator_node.h"

polyadic_multiplication_equality_visitor::polyadic_multiplication_equality_visitor(
polyadic_multiplication_operator_node* node1,bool& retVal) : cascaded_equality_base_visitor(retVal), node1(node1){

}


void polyadic_multiplication_equality_visitor::visit(polyadic_multiplication_operator_node* node2) {
  std::list<expression_node*>::const_iterator iter1 = node1->children.begin();
  std::list<expression_node*>::iterator end1 = node1->children.end();
  std::list<expression_node*>::const_iterator iter2 = node2->children.begin();
  std::list<expression_node*>::iterator end2 = node2->children.end();
  for(;iter1 != end1; ++iter1, ++iter2) {
    if(iter2 == end2) {
      /*We have reached the end of the children of the second node!*/
      retVal = false;
      return;
    }
    if(**iter1 != **iter2) {
      /*The children are not equal*/
      retVal = false;
      return;
    }
  }
  /*Iter1 is now at the end. Iter2 should be at the end too*/
  if(iter2 == end2)
    retVal = true;
  else
    retVal = false;
}
