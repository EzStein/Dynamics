#ifndef FACTORIAL_OPERATOR_NODE_GUARD
#define FACTORIAL_OPERATOR_NODE_GUARD
#include "compiler/ast/unary_operator_node.h"
#include "compiler/ast/expression_node.h"
#include "compiler/compiler_data.h"
class factorial_operator_node : public unary_operator_node {
public:
  /*Constructs this operator node with a pointer to its child node*/
  explicit factorial_operator_node(expression_node*);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual std::ostream& emit_code_ia32(std::ostream&, compiler_data&) const override;
  virtual std::ostream& emit_code_amd64(std::ostream&, compiler_data&) const override;
  virtual unsigned int code_size() const override;
  virtual expression_node* copy() const override;
};
#endif
