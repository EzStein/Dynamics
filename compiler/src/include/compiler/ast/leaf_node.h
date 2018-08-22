#ifndef LEAF_NODE_GUARD
#define LEAF_NODE_GUARD
#include "compiler/ast/expression_node.h"
class leaf_node : public expression_node {
public:
  virtual ~leaf_node();
  expression_node* transform_operators() override;
  expression_node* level_operators() override;
  expression_node* make_pre_canonical() override;
  void sort() override;
  expression_node* collect_terms() override;
  expression_node* optimization_round() override;




};
#endif
