#include "compiler/ast/unary_operator_node.h"
unary_operator_node::unary_operator_node(expression_node* _child) : child(_child) {

}

unary_operator_node::~unary_operator_node() { }

bool unary_operator_node::evaluatable() const {
  return child->evaluatable();
}
