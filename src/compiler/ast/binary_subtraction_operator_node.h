#ifndef BINARY_MINUS_OPERATOR_NODE_GUARD
#define BINARY_MINUS_OPERATOR_NODE_GUARD
#include "compiler/ast/binary_operator_node.h"
#include "compiler/ast/expression_node.h"
#include "compiler/compiler_data.h"

class binary_subtraction_operator_node : public binary_operator_node {
public:
  /*Constructs this operator node with a pointer to its left and right child nodes*/
  binary_subtraction_operator_node(expression_node* leftChild, expression_node* rightChild);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual std::ostream& emit_code_ia32(std::ostream&, compiler_data&) const override;
  virtual std::ostream& emit_code_amd64(std::ostream&, compiler_data&) const override;
  virtual unsigned int code_size() const override;
  virtual expression_node* copy() const override;
  virtual bool is_integral() const override;
  expression_node* transform_operators() override;
  void accept(visitor* v) override;
  expression_node* optimization_round() override;
  expression_node* differentiate(const std::string&) override;


};
#endif
