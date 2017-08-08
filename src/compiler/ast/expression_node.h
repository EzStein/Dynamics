#ifndef EXPRESSION_NODE_GUARD
#define EXPRESSION_NODE_GUARD
#include <iostream>
#include "compiler/ast/node.h"
#include "compiler/compiler_data.h"
class expression_node : public node {
public:
  virtual ~expression_node() = 0;
  /*
  * Emits code by writing assembly to the output stream and filling the provided buffer with bytes
  * starting at the provided offset. The function should modify offset to reflect the bytes added.
  */
  virtual std::ostream& emit_code(std::ostream&, compiler_data&) const = 0;
  virtual double evaluate() const = 0;

  /*True if there are no variables in the sub tree whose root is this node*/
  virtual bool evaluatable() const = 0;
  virtual std::ostream& print(std::ostream&) const = 0;

  /*
  * Copies the tree whose root is this node and returns a pointer to this copy
  */
  virtual expression_node* copy() const = 0;

  /*
  * Returns the size of the produced code.
  */
  virtual unsigned int code_size() const = 0;
};
#endif
