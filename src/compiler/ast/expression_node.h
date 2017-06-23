#ifndef EXPRESSION_NODE_GUARD
#define EXPRESSION_NODE_GUARD
#include "compiler/ast/node.h"
class expression_node : public node {
public:
  virtual ~expression_node();
  virtual double evaluate() const = 0;
};
#endif
