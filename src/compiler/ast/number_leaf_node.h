#ifndef NUMBER_LEAF_NODE_GUARD
#define NUMBER_LEAF_NODE_GUARD
#include "compiler/ast/leaf_node.h"
class number_leaf_node : public leaf_node {
public:
  /*Constructs this leaf node with its value*/
  explicit number_leaf_node(double val);
private:
  virtual double evaluate() const override;
  double val;
};
#endif
