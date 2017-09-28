#include "compiler/ast/expression_node.h"
expression_node::~expression_node() { }

bool operator==(expression_node& node1, expression_node& node2) {
  bool ret;
  equality_visitor visit(&node1, ret);
  (&node2)->accept(&visit);
  return ret;
}


bool operator!=(expression_node& node1, expression_node& node2) {
  return ! (node1 == node2);
}
