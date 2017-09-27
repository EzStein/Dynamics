#ifndef BINARY_OPERATOR_NODE_GUARD
#define BINARY_OPERATOR_NODE_GUARD
#include "compiler/ast/expression_node.h"
class binary_operator_node : public expression_node {
public:
  virtual ~binary_operator_node();

  /*Currently these are public but friend methods should be used instead*/
  expression_node* leftChild;
  expression_node* rightChild;
protected:
  /*Constructs this binary operator node with pointers to its left and right children*/
  binary_operator_node(expression_node* leftChild, expression_node* rightChild);

  virtual bool evaluatable() const override;
  virtual expression_node* transform_negation() override;
  
  expression_node* level_operators() override;
  expression_node* make_pre_canonical() override;


};
#endif
