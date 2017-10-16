/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pre_canonical_exponentiation_operator_visitor.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"

pre_canonical_exponentiation_operator_visitor::pre_canonical_exponentiation_operator_visitor(
expression_node*& _result, exponentiation_operator_node* _parent) : result(_result), parent(_parent){ }

/*
 * In this case we 
 * level the operator like normal.
 */
void pre_canonical_exponentiation_operator_visitor::visit(polyadic_multiplication_operator_node* node) {
  std::list<expression_node*>::iterator iter = node->children.begin();
  std::list<expression_node*>::const_iterator end = node->children.end();
  for(; iter != end; ++iter) {
    /*Since the node is in canonical form,
     its children are exponential nodes*/
    exponentiation_operator_node* child = dynamic_cast<exponentiation_operator_node*>(*iter);
    /*We change the exponent to reflect multiplication*/
    std::list<expression_node*> newExpTerms;
    newExpTerms.push_back(child->rightChild);
    newExpTerms.push_back(parent->rightChild);
    child->rightChild = new polyadic_multiplication_operator_node(newExpTerms);
  }
  parent->leftChild = nullptr;
  parent->rightChild = nullptr;
  result = node;
}

/*
 * For leaf nodes we don't need to change anything
 */
void pre_canonical_exponentiation_operator_visitor::visit(integer_number_leaf_node* node) {
  parent->leftChild = node;
  result = parent;
}

void pre_canonical_exponentiation_operator_visitor::visit(number_leaf_node* node) {
  parent->leftChild = node;
  result = parent;
}

void pre_canonical_exponentiation_operator_visitor::visit(variable_leaf_node* node) {
  parent->leftChild = node;
  result = parent;
}


void pre_canonical_exponentiation_operator_visitor::visit(exponentiation_operator_node* node) {
  std::list<expression_node*> newExpTerms;
  newExpTerms.push_back(node->rightChild);
  newExpTerms.push_back(parent->rightChild);
  node->rightChild = new polyadic_multiplication_operator_node(newExpTerms);
  parent->leftChild = nullptr;
  parent->rightChild = nullptr;
  result = node;
}

void pre_canonical_exponentiation_operator_visitor::visit(polyadic_addition_operator_node* node) {
  /*If the exponent is evaluatable and integer type then we expand the
   expression using the Binomial expansion.
   Otherwise we do essentially nothing*/
  
  long exp = parent->rightChild->evaluate_as_integer();
  if(parent->rightChild->evaluatable() && parent->rightChild->is_integral() && exp > 0) {
    
    std::list<expression_node*> newTerms;
    for(int i = 0; i != exp; ++i) {
      newTerms.push_back(node->copy());
    }
    /*We do not set the left child to nullptr because we want it to be deleted
     since we only made copies of it. We also delete the rightChid because the
     exponent does not manifest in the resulting expression*/
    result = new polyadic_multiplication_operator_node(newTerms);
    return;
  }
  
  parent->leftChild = node;
  result = parent;
}

void pre_canonical_exponentiation_operator_visitor::visit(binary_multiplication_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_exponentiation_operator_visitor::visit(unary_minus_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_exponentiation_operator_visitor::visit(binary_addition_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_exponentiation_operator_visitor::visit(binary_subtraction_operator_node* node) {
  throw "Cannot visit this node";
}

void pre_canonical_exponentiation_operator_visitor::visit(division_operator_node* node) {
  throw "Cannot visit this node";
}

