#include "compiler/ast/unary_minus_operator_node.h"
unary_minus_operator_node::unary_minus_operator_node(expression_node* child) :
unary_operator_node(child) {

}

double unary_minus_operator_node::evaluate() const {
  return -child->evaluate();
}
