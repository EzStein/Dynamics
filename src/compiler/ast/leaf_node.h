#ifndef LEAF_NODE_GUARD
#define LEAF_NODE_GUARD
#include "compiler/ast/expression_node.h"
class leaf_node : public expression_node {
public:
  virtual ~leaf_node();
  expression_node* transform_negation() override;
  expression_node* level_operators() override;
  expression_node* make_pre_canonical() override;


};
#endif
