#ifndef MULTIPLY_OPERATOR_NODE_GUARD
#define MULTIPLY_OPERATOR_NODE_GUARD
#include "compiler/ast/binary_operator_node.h"
#include "compiler/ast/expression_node.h"
class multiply_operator_node : public binary_operator_node {
public:
  /*Constructs this operator node with a pointer to its left and right child nodes*/
  multiply_operator_node(expression_node* leftChild, expression_node* rightChild);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual std::ostream& emit_code(std::ostream&, unsigned char *, unsigned int &) const override;
  virtual unsigned int code_size() const override;
};
#endif
