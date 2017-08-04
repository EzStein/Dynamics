#ifndef UNARY_OPERATOR_NODE_GUARD
#define UNARY_OPERATOR_NODE_GUARD
#include "compiler/ast/expression_node.h"
class unary_operator_node : public expression_node {
public:
  virtual ~unary_operator_node();

protected:
  expression_node* child;

  /*Constructs this unary operator node with a pointer to its child*/
  explicit unary_operator_node(expression_node* child);

private:
  virtual bool evaluatable() const override;
};
#endif
