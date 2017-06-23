#ifndef EXPONENTIATION_OPERATOR_MODE_GUARD
#define EXPONENTIATION_OPERATOR_MODE_GUARD
#include "compiler/ast/binary_operator_node.h"
#include "compiler/ast/expression_node.h"
class exponentiation_operator_node : public binary_operator_node {
public:
  /*Constructs this operator node with a pointer to its left and right child nodes*/
  exponentiation_operator_node(expression_node* leftChild, expression_node* rightChild);
private:
  virtual double evaluate() const override;
};
#endif
