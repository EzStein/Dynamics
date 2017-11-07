#ifndef EXPRESSION_NODE_GUARD
#define EXPRESSION_NODE_GUARD
#include <iostream>
#include <iomanip>
#include "compiler/ast/node.h"
#include "compiler/compiler_data.h"
#include "compiler/ast/visitor/visitor.h"
#include "compiler/ast/visitor/comparator/std_node_comparator.h"
#include "compiler/ast/visitor/comparator/canonical_node_comparator.h"

class expression_node : public node {
public:
  virtual ~expression_node() = 0;

  /*
  * Appends assembly language to the end of the provided string.
  */
  virtual void emit_code_amd64(std::string&, compiler_data&) const = 0;
  virtual double evaluate() const = 0;
  virtual long evaluate_as_integer() const = 0;

  /*True if there are no variables in the sub tree whose root is this node*/
  virtual bool evaluatable() const = 0;
  virtual std::ostream& print(std::ostream&) const = 0;

  /*
  * Copies the tree whose root is this node and returns a pointer to this copy
  */
  virtual expression_node* copy() const = 0;

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
  virtual void accept(visitor* v) = 0;

  /**
   * When this method is called on a node,
   * the node transforms itself so that it contains
   * no binary or unary minus operator nodes or division operators. The node
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
  virtual expression_node* transform_operators() = 0;

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


  /**
   * This method should ONLY be called on subtrees
   * whose nodes are one of the following:
   * polyadic multiplication
   * polyadic addition
   * exponentiation
   * integer number
   * number
   * variable
   *
   * This method puts the subtree whose root is 'this'
   * into pre-canonical form. A subtree is in Pre-canonical if it satisfies the following:
   * All children of polyadic addition nodes are polyadic multiplication nodes
   * All children of polyadic multiplication nodes are exponentiation nodes.
   * The exponent of an exponentiation node is an expression in precanonical form.
   * If the exponent is evaluatable and of integer value, then its
   * base is a leaf node. Otherwise the base may be a leaf node OR a polyadic addition node.
   * and all its subtrees are in precanonical form.
   *
   * Currently simplification to precanonical form does not
   * support division operations or exponential operators whose exponent is not
   * an integer number leaf node.
   * @return
   */
  virtual expression_node* make_pre_canonical() = 0;


  /*
   * Sorts the given node by
   * ordering its children according to the standard comparator
   * operator between nodes. Note that there are no preconditions
   * on the type of tree for this function but it is often called
   * after make_pre_canonical to prepare for term collections.
   * Note that only commutative nodes should have a nontrivial
   * implementation of this method.
   */
  virtual void sort() = 0;


  /**
   * This method collects like terms together.
   * It will return an expression_node representing the node
   * that should replace the one it was called. Sometimes it
   * returns the same expression_node* as the node that it
   * was called on in which case the old node should not be deleted.
   * If a different node was returned the old node should be deleted by the caller.
   *
   * Precondition:
   * The node should be in pre-canonical form and
   * should be sorted according to canonical ordering.
   *
   * The node that is returned by this method will still be in
   * pre-canonical form and will still be ordered according to
   * canonical ordering. Additionally, polyadic types will be maintained.
   * @return
   */
  virtual expression_node* collect_terms() = 0;

  /**
   * Performs one round of optimization on the node and its subtrees.
   * In general, this function should recursively call optimization_round()
   * on each of its children, and then perform EXACTLY one of the following optimizations:
   *
   * If the node is evaluatable it is replaced with its evaluation.
   * If a polyadic node has one child, the polyadic node is replaced by that child.
   * Note that this must happen before any other optimization checks for otherwise
   * it would be possible for a polyadic child to have no children.
   *
   * In a multiplication node, any children that evaluate to 1 exactly
   * will be removed. Note that in this evaluation children whose type is
   * of integer type should be evaluated and then ROUNDED to an integer before comparing it to 1.
   * If any children that evaluate to zero the function returns an integer_number_leaf
   * whose value is zero.
   *
   * In an addition node, any children that evaluate to zero as above will be removed
   * in a similar fashion as above.
   *
   * In a subtraction node, if the left child evaluates to zero,
   * node is replaced by the unary minus of the left child.
   * If the right child is zero the node is replaced by the right child.
   *
   * In a division node, if the rightChild is 1 it is replaced
   * by a the leftChild. If the leftChild is zero, it is replaced by zero
   * if the right child is not also zero.
   *
   * If a binary multiplication node has only one child as a result
   * of the above, it's child replaces it.
   *
   * In an exponentiation node, an exponent that evaluates to 1 will be replaced by its base.
   * An exponent that evaluates to zero will be replaced by a node representing 1
   * If its base does not also evaluate to zero.
   * If its base evaluates to zero the node will be replaced by
   * a zero if the exponent does not also evaluate to zero.
   * If the base evaluates to 1, it will be replaced by 1.
   *
   * Leaf nodes are already optimized and should return this.
   *
   * If the child of a unary minus operator is evaluatable,
   * it should be replaced with the appropriate value which
   * may or may not be integral.
   *
   * In general this method is called on the root of the AST until
   * the AST does not change. At that point, the AST is considered optimized.
   *
   * As with the previous methods, if the callee does not return this,
   * then the caller will delete the node that it called.
   *
   * Note that optimization_round will not introduce non-precanonical nodes
   * if the original tree is does not have those nodes.
   *
   *
   * @return
   */
  virtual expression_node* optimization_round() = 0;

  /*
   * Differentiates the expression
   * with respect to the variable given in the string.
   * We assume that only the following nodes are present as in canonical form:
   *
   * polyadic multiplication
   * polyadic addition
   * exponentiation
   * integer number
   * number
   * variable
   *
   * When a number node is differentiated it returns a zero integer number node.
   * When a variable is differentiated it returns 1 if it is the number we are differentiating
   * and zero otherwise.
   * Polyadic addition simply returns the sum of the derivatives of each term.
   * polyadic multiplication uses the product rule to return the result.
   *
   * Left F be the base and G be the exponent. Then
   * the derivative is
   * F^G(GF^(-1)F' + G'LN(F))
   * This is derived using implicit differentiation.
   *
   *
   * THUS: precondition for this method
   * is that only these operators are used. This
   * is guarenteed if transform_operators followed by
   * level_operators is called. Note that
   * the result of this function is a tree
   * that contains only the proper nodes,
   * but the operators themselves may not be leveled.
   * So simplification should then be performed as
   * usual.
   */
  virtual expression_node* differentiate(const std::string&) = 0;
};

/*Here we add in comparators for expression nodes
*NOte that these should be passed by CONST expression in the future*/
bool operator==(expression_node& node1, expression_node& node2);
bool operator!=(expression_node& node1, expression_node& node2);
bool operator<(expression_node& node1, expression_node& node2);
bool operator>(expression_node& node1, expression_node& node2);
bool operator<=(expression_node& node1, expression_node& node2);
bool operator>=(expression_node& node1, expression_node& node2);
#endif
