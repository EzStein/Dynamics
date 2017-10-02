#ifndef UNARY_OPERATOR_NODE_GUARD
#define UNARY_OPERATOR_NODE_GUARD
#include "compiler/ast/expression_node.h"
class unary_operator_node : public expression_node {
public:
  virtual ~unary_operator_node();

  /*Should be made protected and use friend class instead*/
  expression_node* child;
protected:


  /*Constructs this unary operator node with a pointer to its child*/
  explicit unary_operator_node(expression_node* child);
  expression_node* transform_negation() override;
  expression_node* level_operators() override;
  expression_node* make_pre_canonical() override;
  void sort() override;
  expression_node* collect_terms() override;
  expression_node* optimization_round() override;






private:
  virtual bool evaluatable() const override;

};
#endif
