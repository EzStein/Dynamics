#ifndef VARIABLE_LEAF_NODE_GUARD
#define VARIABLE_LEAF_NODE_GUARD
#include <list>
#include "compiler/ir/symbol.h"
#include "compiler/ast/leaf_node.h"
#include "compiler/compiler_data.h"
class variable_leaf_node : public leaf_node {
public:
  /*Constructs this leaf node with a pointer to its symbol table entry*/
  explicit variable_leaf_node(symbol::ptr_type);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual void emit_code_amd64(std::string&, compiler_data&) const override;
  virtual bool evaluatable() const override;
  virtual expression_node* copy() const override;
  bool is_integral() const override;
  void accept(visitor* v) override;
  expression_node* differentiate(const std::string& var) override;


/*Make this friend*/
public:
  symbol::ptr_type symPtr;
};

#endif
