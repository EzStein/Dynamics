/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
*/
#include <iostream>
#include "compiler/ast/visitor/comparator/std_node_comparator.h"
#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include  "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"

void std_node_comparator::visit(binary_addition_operator_node* node1, binary_addition_operator_node* node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(node1->leftChild, node2->leftChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  comp.dispatch(node1->rightChild, node2->rightChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  retVal = 0;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, binary_subtraction_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, unary_minus_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, division_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node2->children.size() < 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node2->children.size() < 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_addition_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, binary_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, binary_subtraction_operator_node* node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(node1->leftChild, node2->leftChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  comp.dispatch(node1->rightChild, node2->rightChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  retVal = 0;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, unary_minus_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, division_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node2->children.size() < 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node2->children.size() < 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_subtraction_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}


void std_node_comparator::visit(binary_multiplication_operator_node* node1, binary_addition_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, binary_subtraction_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, binary_multiplication_operator_node* node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(node1->leftChild, node2->leftChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  comp.dispatch(node1->rightChild, node2->rightChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  retVal = 0;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, unary_minus_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, division_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node2->children.size() <= 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node2->children.size() < 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(binary_multiplication_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, binary_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, binary_subtraction_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, unary_minus_operator_node* node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(node1->child, node2->child);
  retVal = val;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, division_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, polyadic_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(unary_minus_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}


void std_node_comparator::visit(exponentiation_operator_node* node1, binary_addition_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, binary_subtraction_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, binary_multiplication_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, unary_minus_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, exponentiation_operator_node* node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(node1->leftChild, node2->leftChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  comp.dispatch(node1->rightChild, node2->rightChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  retVal = 0;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, division_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node2->children.size() <= 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node2->children.size() <= 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(exponentiation_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(division_operator_node* node1, binary_addition_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(division_operator_node* node1, binary_subtraction_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(division_operator_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(division_operator_node* node1, unary_minus_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(division_operator_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(division_operator_node* node1, division_operator_node* node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(node1->leftChild, node2->leftChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  comp.dispatch(node1->rightChild, node2->rightChild);
  if(val != 0) {
    retVal = val;
    return;
  }

  retVal = 0;
}

void std_node_comparator::visit(division_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node2->children.size() <= 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(division_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node2->children.size() < 2)
    retVal = 1;
  else
    retVal = -1;
}

void std_node_comparator::visit(division_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(division_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(division_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, binary_addition_operator_node* node2) {
  if(node1->children.size() < 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, binary_subtraction_operator_node* node2) {
  if(node1->children.size() < 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, binary_multiplication_operator_node* node2) {
  if(node1->children.size() <= 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, unary_minus_operator_node* node2) {
  if(node1->children.size() < 1)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, exponentiation_operator_node* node2) {
  if(node1->children.size() <= 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, division_operator_node* node2) {
  if(node1->children.size() <= 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node1->children.size() < node2->children.size()) {
    retVal = -1;
  } else if(node1->children.size() > node2->children.size()) {
    retVal = 1;
  } else {
    /*Same number of children*/
    std::list<expression_node*>::const_iterator iter1 = node1->children.begin();
    std::list<expression_node*>::const_iterator end1 = node1->children.end();
      std::list<expression_node*>::const_iterator iter2 = node2->children.begin();
    for(; iter1 != end1; ++iter1, ++iter2) {
      int val;
      std_node_comparator comp(val);
      /*Calculate the relationship between the two children*/
      comp.dispatch(*iter1, *iter2);
      if(val != 0) {
        retVal = val;
        return;
      }
    }
    retVal = 0;
  }
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node1->children.size() <= node2->children.size())
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, integer_number_leaf_node* node2) {
  if(node1->children.size() < 1)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, number_leaf_node* node2) {
  if(node1->children.size() < 1)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_addition_operator_node* node1, variable_leaf_node* node2) {
  if(node1->children.size() < 1)
    retVal = -1;
  else
    retVal = 1;
}


void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, binary_addition_operator_node* node2) {
  if(node1->children.size() < 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, binary_subtraction_operator_node* node2) {
  if(node1->children.size() < 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, binary_multiplication_operator_node* node2) {
  if(node1->children.size() < 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, unary_minus_operator_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, exponentiation_operator_node* node2) {
  if(node1->children.size() <= 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, division_operator_node* node2) {
  if(node1->children.size() < 2)
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, polyadic_addition_operator_node* node2) {
  if(node1->children.size() < node2->children.size())
    retVal = -1;
  else
    retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) {
  if(node1->children.size() < node2->children.size()) {
    retVal = -1;
  } else if(node1->children.size() > node2->children.size()) {
    retVal = 1;
  } else {
    /*Same number of children*/
    std::list<expression_node*>::const_iterator iter1 = node1->children.begin();
    std::list<expression_node*>::const_iterator end1 = node1->children.end();
      std::list<expression_node*>::const_iterator iter2 = node2->children.begin();
    for(; iter1 != end1; ++iter1, ++iter2) {
      int val;
      std_node_comparator comp(val);
      /*Calculate the relationship between the two children*/
      comp.dispatch(*iter1, *iter2);
      if(val != 0) {
        retVal = val;
        return;
      }
    }
    retVal = 0;
  }
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(polyadic_multiplication_operator_node* node1, variable_leaf_node* node2) {
  retVal = 1;
}


void std_node_comparator::visit(integer_number_leaf_node* node1, binary_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, binary_subtraction_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, unary_minus_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, division_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, polyadic_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, polyadic_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, integer_number_leaf_node* node2) {
  if(node1->val < node2->val)
    retVal = -1;
  else if(node1->val == node2->val)
    retVal = 0;
  else
    retVal = 1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, number_leaf_node* node2) {
  if(node1->val < node2->val)
    retVal = -1;
  else if(node1->val == node2->val)
    retVal = 0;
  else
    retVal = 1;
}

void std_node_comparator::visit(integer_number_leaf_node* node1, variable_leaf_node* node2) {
  retVal = -1;
}


void std_node_comparator::visit(number_leaf_node* node1, binary_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, binary_subtraction_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, unary_minus_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, division_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, polyadic_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, polyadic_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(number_leaf_node* node1, integer_number_leaf_node* node2) {
  if(node1->val < node2->val)
    retVal = -1;
  else if(node1->val == node2->val)
    retVal = 0;
  else
    retVal = 1;
}

void std_node_comparator::visit(number_leaf_node* node1, number_leaf_node* node2) {
  if(node1->val < node2->val)
    retVal = -1;
  else if(node1->val == node2->val)
    retVal = 0;
  else
    retVal = 1;
}

void std_node_comparator::visit(number_leaf_node* node1, variable_leaf_node* node2) {
  retVal = -1;
}


void std_node_comparator::visit(variable_leaf_node* node1, binary_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, binary_subtraction_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, binary_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, unary_minus_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, exponentiation_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, division_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, polyadic_addition_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, polyadic_multiplication_operator_node* node2) {
  retVal = -1;
}

void std_node_comparator::visit(variable_leaf_node* node1, integer_number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(variable_leaf_node* node1, number_leaf_node* node2) {
  retVal = 1;
}

void std_node_comparator::visit(variable_leaf_node* node1, variable_leaf_node* node2) {
  if(node1->symPtr->name < node2->symPtr->name)
    retVal = -1;
  else if(node1->symPtr->name == node2->symPtr->name)
    retVal = 0;
  else
    retVal = 1;
}
