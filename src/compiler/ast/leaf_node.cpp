#include "compiler/ast/leaf_node.h"
leaf_node::~leaf_node() {
}

expression_node* leaf_node::transform_negation() {
  return this;
}

expression_node* leaf_node::level_operators() {
  return this;
}

expression_node* leaf_node::make_pre_canonical() {
  return this;
}

void leaf_node::sort() {
  /*Does nothing since leaf nodes have no children*/
}


