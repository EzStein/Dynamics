#ifndef MULTIPLY_OPERATOR_NODE_GUARD
#define MULTIPLY_OPERATOR_NODE_GUARD
#include "compiler/ast/binary_operator_node.h"
#include "compiler/ast/expression_node.h"
#include "compiler/compiler_data.h"
#include "compiler/ast/visitor/mutation/level_multiplication_operator_visitor.h"
class binary_multiplication_operator_node : public binary_operator_node {
public:
  /*Constructs this operator node with a pointer to its left and right child nodes*/
  binary_multiplication_operator_node(expression_node* leftChild, expression_node* rightChild);
  friend level_multiplication_operator_visitor;
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual void emit_code_amd64(std::string&, compiler_data&) const override;
  virtual expression_node* copy() const override;
  virtual bool is_integral() const override;
  void accept(visitor* v) override;
  expression_node* level_operators() override;
  void sort() override;
  expression_node* optimization_round() override;
  expression_node* differentiate(const std::string&) override;


};
#endif
