#include "compiler/ast/leaf_node.h"
leaf_node::~leaf_node() {
}

expression_node* leaf_node::transform_negation() {
  return this;
}
