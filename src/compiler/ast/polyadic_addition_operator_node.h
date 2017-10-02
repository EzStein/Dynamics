/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   polyadic_addition_operator_node.h
 * Author: ezra
 *
 * Created on September 25, 2017, 7:31 PM
 * Represents an addition operation performed over many children.
 * That is, it sums its children.
 */

#ifndef POLYADIC_ADDITION_OPERATOR_NODE_H
#define POLYADIC_ADDITION_OPERATOR_NODE_H
#include "compiler/ast/polyadic_operator_node.h"

class polyadic_addition_operator_node : public polyadic_operator_node {
public:
    /**
     * Constructs this node from a list of children.
     * NOTE: this list MUST not be empty.
     * @param
     */
    polyadic_addition_operator_node(std::list<expression_node*>);

  polyadic_addition_operator_node(expression_node* firstChild);
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;
  virtual std::ostream& emit_code_ia32(std::ostream&, compiler_data&) const override;
  virtual std::ostream& emit_code_amd64(std::ostream&, compiler_data&) const override;
  virtual unsigned int code_size() const override;
  virtual expression_node* copy() const override;
  bool is_integral() const override;
  void accept(visitor* v) override;
  expression_node* level_operators() override;
  expression_node* make_pre_canonical() override;
  expression_node* collect_terms() override;
  expression_node* optimization_round() override;





};

#endif /* POLYADIC_ADDITION_OPERATOR_NODE_H */
