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
    val &&= (*iter)->evaluatable();
  }
  return val;
}

