/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   integer_number_leaf_node.h
 * Author: ezra
 *
 * Created on September 25, 2017, 6:25 PM
 *
 * This node represents a determined number which is known to be an integer.
 * Internally the integer is stored as a long. Note that
 * some operations are different if they are performed on
 * nodes which are known to have integral value.
 */

#ifndef INTEGER_NUMBER_LEAF_NODE_H
#define INTEGER_NUMBER_LEAF_NODE_H
#include "compiler/ast/leaf_node.h"
class integer_number_leaf_node : public leaf_node {
public:
  integer_number_leaf_node(long);

  virtual std::ostream& emit_code_ia32(std::ostream&, compiler_data&) const override;
  virtual std::ostream& emit_code_amd64(std::ostream&, compiler_data&) const override;
  virtual double evaluate() const override;

  /*True if there are no variables in the sub tree whose root is this node*/
  virtual bool evaluatable() const override;
  virtual std::ostream& print(std::ostream&) const override;

  /*
  * Copies the tree whose root is this node and returns a pointer to this copy
  */
  virtual expression_node* copy() const override;

  /*
  * Returns the size of the produced code.
  */
  virtual unsigned int code_size() const override;

  /*Make friend instead of public*/
  long val;
private:

  virtual bool is_integral() const override;
  void accept(visitor& v) override;

};

#endif /* INTEGER_NUMBER_LEAF_NODE_H */
