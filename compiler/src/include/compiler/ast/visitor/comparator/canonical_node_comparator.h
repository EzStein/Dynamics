/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   canonical_node_comparator.h
 * Author: ezra
 *
 * Created on September 29, 2017, 12:23 AM
 *
 * A comparator that overrides some std_comparator
 * functionality that will be useful in developing a canonical
 * form.
 * The only change is that exponentiation nodes
 * that are evaluatable will be marked as LESS than any non evaluatable
 * exponentiation node regardless of how many children it has.
 * Two nodes that are both evaluatable or both non-evaluatable will use the usual comparison methods.
 * For exponentiation nodes, this means that the base is compared first, so they are ordered by base.
 * 
 * polyadic multiplication will only be compared by their nonevaluatable part
 * If two polyadic nodes are both evaluatable they are compared normally.
 * If one is evaluatable and another isn't, the evaluatable one is considered LESS than the other.
 * If neither are evaluatable they are compared normally by their non-evaluatable parts.
 */

#ifndef CANONICAL_NODE_COMPARATOR_H
#define CANONICAL_NODE_COMPARATOR_H

#include "compiler/ast/visitor/comparator/std_node_comparator.h"
class canonical_node_comparator : public std_node_comparator {
public:
  canonical_node_comparator(int& retVal) : std_node_comparator(retVal) {};

  void visit(exponentiation_operator_node* node1, exponentiation_operator_node* node2) override;
  void visit(polyadic_multiplication_operator_node* node1, polyadic_multiplication_operator_node* node2) override;
};


#endif /* CANONICAL_NODE_COMPARATOR_H */
