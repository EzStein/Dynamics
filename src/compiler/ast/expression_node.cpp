#include "compiler/ast/expression_node.h"
expression_node::~expression_node() { }

bool operator==(expression_node& node1, expression_node& node2) {
  int val;
  canonical_node_comparator comp(val);
  comp.dispatch(&node1, &node2);
  return val == 0;
}

bool operator!=(expression_node& node1, expression_node& node2) {
  int val;
  canonical_node_comparator comp(val);
  comp.dispatch(&node1, &node2);
  return val != 0;
}

bool operator<(expression_node& node1, expression_node& node2) {
  int val;
  canonical_node_comparator comp(val);
  comp.dispatch(&node1, &node2);
  return val < 0;
}

bool operator>(expression_node& node1, expression_node& node2) {
  int val;
  canonical_node_comparator comp(val);
  comp.dispatch(&node1, &node2);
  return val > 0;
}

bool operator>=(expression_node& node1, expression_node& node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(&node1, &node2);
  return val >= 0;
}

bool operator<=(expression_node& node1, expression_node& node2) {
  int val;
  std_node_comparator comp(val);
  comp.dispatch(&node1, &node2);
  return val <= 0;
}

