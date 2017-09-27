#ifndef EXPRESSION_NODE_GUARD
#define EXPRESSION_NODE_GUARD
#include <iostream>
#include "compiler/ast/node.h"
#include "compiler/compiler_data.h"
#include "compiler/ast/visitor/visitor.h"

class expression_node : public node {
public:
  virtual ~expression_node() = 0;
  /*
  * Emits code by writing assembly to the output stream and filling the provided buffer with bytes
  * starting at the provided offset. The function should modify offset to reflect the bytes added.
  */
  virtual std::ostream& emit_code_ia32(std::ostream&, compiler_data&) const = 0;
  virtual std::ostream& emit_code_amd64(std::ostream&, compiler_data&) const = 0;
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

  /*
  * Returns true if this node is known to be an integral type.
  * Otherwise, returns false if it may or may not be an integral type.
  */
  virtual bool is_integral() const = 0;

  /**
   * Used in the visitor pattern.
   * Only the most derived classes should implement this method,
   * and all they should do is call visit(this) on
   * the visitor.
   * @param
   */
  virtual void accept(visitor& v) = 0;

  /**
   * When this method is called on a node,
   * the node transforms itself so that it contains
   * no binary or unary minus operator nodes. The node
   * itself may need to change type, so it returns
   * a pointer to the node that should replace it as a child.
   * If the node returns a pointer that is different from the
   * the current child, then we assume that that the original
   * child is no longer in use and should be deleted. Thus
   * if this function does not return this,
   * it will be deleted by the calling function. It is
   * important that if the function does not return this,
   * it sets its own children to null so that they are not deleted.
   * @return
   */
  virtual expression_node* transform_negation() = 0;

  /**
   * When this method is called on a node,
   * the node is transformed so that cascaded multiplication
   * and addition operators are leveled into one polyadic operator.
   * A multiplication and addition node is considered to be leveled if if none of its children
   * are of the same type as the node (as in the same operator type, + or *). This will also turn any binary operators
   * into polyadic ones even if there is no cascading.
   * The function returns a pointer to a new node that will replace
   * this as a child. If this new node is different then the old one,
   * the old one should be deleted by the caller.
   * If the new node is a polyadic operator of the appropriate type,
   * and the current node is an operator of the same type,
   * then a visitor is used to incorporate the children of the returned
   * node into the current one.
   * @return
   */
  virtual expression_node* level_operators() = 0;

};
#endif
