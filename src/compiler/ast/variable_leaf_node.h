#ifndef VARIABLE_LEAF_NODE_GUARD
#define VARIABLE_LEAF_NODE_GUARD
#include "compiler/ast/leaf_node.h"
class variable_leaf_node : public leaf_node {
public:
  /*Constructs this leaf node with its id*/
  explicit variable_leaf_node(unsigned int id);
private:
  virtual double evaluate() const override;
  unsigned int id;
};

#endif
