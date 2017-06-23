#include "compiler/ast/number_leaf_node.h"
number_leaf_node::number_leaf_node(double _val) : val(_val) {

}

double number_leaf_node::evaluate() const {
  return val;
}
