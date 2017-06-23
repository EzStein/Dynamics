#include "compiler/ast/variable_leaf_node.h"
variable_leaf_node::variable_leaf_node(unsigned int _id):id(_id) {

}

double variable_leaf_node::evaluate() const {
  return static_cast<double>(id);
}
