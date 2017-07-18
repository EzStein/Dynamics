#ifndef UNARY_MINUS_OPERATOR_NODE_GUARD
#define UNARY_MINUS_OPERATOR_NODE_GUARD
#include "compiler/ast/unary_operator_node.h"
#include "compiler/ast/expression_node.h"
class unary_minus_operator_node : public unary_operator_node {
public:
  /*Constructs this operator node with a pointer to its child node*/
  explicit unary_minus_operator_node(expression_node*);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual std::ostream& emit_code(std::ostream&) const override;
};
#endif
