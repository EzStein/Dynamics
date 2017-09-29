/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "compiler/ast/visitor/comparator/canonical_node_comparator.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"

void canonical_node_comparator::visit(exponentiation_operator_node* node1,
  exponentiation_operator_node* node2) {
  bool evaluatable1 = node1->evaluatable();
  bool evaluatable2 = node2->evaluatable();
  if(evaluatable1 == evaluatable2) {
    /*The nodes are either both evaluatable or neither,
     so we use the normal method*/
    std_node_comparator::visit(node1, node2);
  } else {
    /*Exactly One of the two is evaluatable*/
    if(evaluatable1)
      retVal = -1;
    else
      retVal = 1;
  }
}

void canonical_node_comparator::visit(polyadic_multiplication_operator_node* node1,
  polyadic_multiplication_operator_node* node2) {
  bool evaluatable1 = node1->evaluatable();
  bool evaluatable2 = node2->evaluatable();
  if(evaluatable1 == evaluatable2) {
    /*The nodes are either both evaluatable or neither,
     so we use the normal method*/
    std_node_comparator::visit(node1, node2);
  } else {
    /*Exactly One of the two is evaluatable*/
    if(evaluatable1)
      retVal = -1;
    else
      retVal = 1;
  }
}