/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <list>
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "expression_node.h"
#include "compiler/ast/visitor/level_multiplication_operator_visitor.h"


 polyadic_multiplication_operator_node::polyadic_multiplication_operator_node(expression_node* firstChild) {
   children.push_back(firstChild);
 }
 
 polyadic_multiplication_operator_node::polyadic_multiplication_operator_node(
 const std::list<expression_node*> _children) : polyadic_operator_node(_children) {}
 
 

 double polyadic_multiplication_operator_node::evaluate() const {
   const_iterator_t iter = children.begin();
   const_iterator_t end = children.end();
   double acc = 1.0;
   for(; iter != end; ++iter) {
     acc *= (*iter)->evaluate();
   }
   return acc;
 }

 std::ostream& polyadic_multiplication_operator_node::print(std::ostream& out) const {
   const_iterator_t iter = children.begin();
   const_iterator_t end = children.end();
   out << '(';
   (*iter)->print(out);
   out << ')';
   ++iter;
   for(; iter != end; ++iter) {
     out << "*";
     out << '(';
     (*iter)->print(out) << ')';
   }
   return out;
 }

 std::ostream& polyadic_multiplication_operator_node::emit_code_ia32(std::ostream& out, compiler_data&) const {
   return out;
 }
 std::ostream& polyadic_multiplication_operator_node::emit_code_amd64(std::ostream& out, compiler_data&) const {
   return out;
 }
 unsigned int polyadic_multiplication_operator_node::code_size() const {
   return 100;
 }

 expression_node* polyadic_multiplication_operator_node::copy() const {
   const_iterator_t iter = children.begin();
   
   const_iterator_t end = children.end();
   /*Add the first child copy to the list*/
   polyadic_multiplication_operator_node* copy
   = new polyadic_multiplication_operator_node((*iter)->copy());
   
   ++iter; /*Start on the second child*/
   for(; iter != end; ++iter) {
     copy->children.push_back((*iter)->copy());
   }
   return copy;
 }
 
bool polyadic_multiplication_operator_node::is_integral() const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  bool val = true;
  for(; iter != end; ++iter) {
    val = val && (*iter)->is_integral();
  }
  return val;
}

void polyadic_multiplication_operator_node::accept(visitor& v) {
  v.visit(this);
}

expression_node* polyadic_multiplication_operator_node::level_operators() {
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* newChild = (*iter)->level_operators();
    if(*iter != newChild)
      delete *iter;
    *iter = newChild;
  }
  std::list<expression_node*> newChildren;
  level_multiplication_operator_visitor visit(newChildren);
  iter = children.begin();
  for(; iter!=end; ++iter) {
    (*iter)->accept(visit);
  }
  children = newChildren;
  return this;
}
