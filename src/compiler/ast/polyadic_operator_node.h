/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   polyadic_operator_node.h
 * Author: ezra
 *
 * Created on September 25, 2017, 7:32 PM
 * This class represents a node with a variable number of children.
 * Any polyadic_operator_node must have atleast one child.
 */

#ifndef POLYADIC_OPERATOR_NODE_H
#define POLYADIC_OPERATOR_NODE_H
#include <list>
#include "compiler/ast/expression_node.h"

class polyadic_operator_node : public expression_node {
public:
  
  polyadic_operator_node() {};
  polyadic_operator_node(const std::list<expression_node*> _children) : children(_children) { };
  virtual ~polyadic_operator_node();
  expression_node* transform_negation() override;
  expression_node* level_operators() override;

  typedef std::list<expression_node*>::const_iterator const_iterator_t;
  typedef std::list<expression_node*>::iterator iterator_t;
  
/* Currently this is set to public, but using friend classes for binary addition and multiplication
 * It can be set to private
 */
public:
  std::list<expression_node*> children;
private:
  bool evaluatable() const override;
  void sort() override;


};



#endif /* POLYADIC_OPERATOR_NODE_H */
