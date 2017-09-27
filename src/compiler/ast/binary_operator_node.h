#ifndef BINARY_OPERATOR_NODE_GUARD
#define BINARY_OPERATOR_NODE_GUARD
#include "compiler/ast/expression_node.h"
class binary_operator_node : public expression_node {
public:
  virtual ~binary_operator_node();

protected:
  expression_node* leftChild;
  expression_node* rightChild;

  /*Constructs this binary operator node with pointers to its left and right children*/
  binary_operator_node(expression_node* leftChild, expression_node* rightChild);

  virtual bool evaluatable() const override;
  virtual expression_node* transform_negation() override;
  
  expression_node* level_operators() override;


};
#endif
