/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   polyadic_multiplication_operator_node.h
 * Author: ezra
 *
 * Created on September 25, 2017, 7:33 PM
 * Node representing multiplication of many children together.
 */

#ifndef POLYADIC_MULTIPLICATION_OPERATOR_NODE_H
#define POLYADIC_MULTIPLICATION_OPERATOR_NODE_H
#include "compiler/ast/polyadic_operator_node.h"
class polyadic_multiplication_operator_node : public polyadic_operator_node {
/*
 *Constructs this node from a list of children.
 * Note that the list MUST be non-empty.
 */
public:
  polyadic_multiplication_operator_node(const std::list<expression_node*> children);

  /*Variable parameter ctor using variadic templates*/
  template<class T>
  polyadic_multiplication_operator_node(T child) {
    children.push_front(child);
  }

  template<class T, class...T2>
  polyadic_multiplication_operator_node(T child, T2... rest) : polyadic_multiplication_operator_node(rest...) {
    children.push_front(child);
  }

  polyadic_multiplication_operator_node(expression_node* firstChild);
  virtual double evaluate() const override;
  virtual std::ostream& print(std::ostream&) const override;

  virtual void emit_code_amd64(std::string&, compiler_data&) const override;
  
  virtual expression_node* copy() const override;
  bool is_integral() const override;
  void accept(visitor* v) override;
  expression_node* level_operators() override;
  expression_node* make_pre_canonical() override;
  expression_node* collect_terms() override;
  expression_node* optimization_round() override;
  expression_node* differentiate(const std::string& var) override;




};

#endif /* POLYADIC_MULTIPLICATION_OPERATOR_NODE_H */
