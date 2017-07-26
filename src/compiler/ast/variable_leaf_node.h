#ifndef VARIABLE_LEAF_NODE_GUARD
#define VARIABLE_LEAF_NODE_GUARD
#include <list>
#include "compiler/ir/symbol.h"
#include "compiler/ast/leaf_node.h"
class variable_leaf_node : public leaf_node {
public:
  typedef std::list<symbol>::const_iterator symbol_ptr_type;
  /*Constructs this leaf node with a pointer to its symbol table entry*/
  explicit variable_leaf_node(symbol_ptr_type);
private:
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual std::ostream& emit_code(std::ostream&, unsigned char *, unsigned int &) const override;
  virtual unsigned int code_size() const override;
  symbol_ptr_type symbolPtr;
};

#endif
