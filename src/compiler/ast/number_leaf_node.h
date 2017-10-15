#ifndef NUMBER_LEAF_NODE_GUARD
#define NUMBER_LEAF_NODE_GUARD
#include "compiler/ast/leaf_node.h"
#include "compiler/compiler_data.h"
#include "compiler/compiler_data.h"
class number_leaf_node : public leaf_node {
public:
  /*Constructs this leaf node with its value*/
  explicit number_leaf_node(double val);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual void emit_code_amd64(std::string&, compiler_data&) const override;
  virtual bool evaluatable() const override;
  virtual expression_node* copy() const override;
  bool is_integral() const override;
  void accept(visitor* v) override;
  expression_node* differentiate(const std::string& var) override;

public:
  double val;
};
#endif
