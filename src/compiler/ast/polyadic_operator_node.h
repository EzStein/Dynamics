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

class polyadic_operator_node : public expression_node {
public:
  virtual ~polyadic_operator_node();
protected:
  typedef std::list<expression_node*>::const_iterator const_iterator_t;
  typedef std::list<expression_node*>::iterator iterator_t;
  std::list<expression_node*> children;
private:
  bool evaluatable() const override;

};



#endif /* POLYADIC_OPERATOR_NODE_H */
