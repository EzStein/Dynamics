#ifndef EXPRESSION_NODE_GUARD
#define EXPRESSION_NODE_GUARD
#include <iostream>
#include "compiler/ast/node.h"
class expression_node : public node {
public:
  virtual ~expression_node();
  virtual std::ostream& emit_code(std::ostream&) const = 0;
  virtual double evaluate() const = 0;
  virtual std::ostream& print(std::ostream&) const = 0;
};
#endif
