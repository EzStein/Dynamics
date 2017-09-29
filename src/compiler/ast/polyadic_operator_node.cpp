#include <algorithm>
#include "compiler/ast/polyadic_operator_node.h"
#include "expression_node.h"
polyadic_operator_node::~polyadic_operator_node() {
  /*We destroy each of the children*/
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    delete *iter;
  }
}

bool polyadic_operator_node::evaluatable() const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  bool val = true;
  for(; iter != end; ++iter) {
    val = val && (*iter)->evaluatable();
  }
  return val;
}

expression_node* polyadic_operator_node::transform_negation() {
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* newChild = (*iter)->transform_negation();
    if(newChild != *iter)
      delete *iter;
    *iter = newChild;
  }
  return this;
}

expression_node* polyadic_operator_node::level_operators() {
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* newChild = (*iter)->level_operators();
    if(newChild != *iter)
      delete *iter;
    *iter = newChild;
  }
  return this;
}

static bool cmp(expression_node*& node1, expression_node*& node2) {
  return *node1 < *node2;
}

void polyadic_operator_node::sort() {
  /*First we call sort on all the children*/
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter!=end; ++iter){
    (*iter)->sort();
  }
  
  /*Then we reorder the children*/
  /*Currently all polyadic nodes are commutative so they are all handled here*/
  children.sort(cmp);
}
