/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <list>
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "expression_node.h"
#include "compiler/ast/visitor/level_multiplication_operator_visitor.h"
#include "compiler/ast/visitor/pre_canonical_multiplication_operator_visitor.h"
#include "compiler/ast/visitor/pre_canonical_multiplication_operator_identifier_visitor.h"
#include "polyadic_addition_operator_node.h"


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
  std::list<expression_node*> newChildren, toDelete;
  level_multiplication_operator_visitor visit(newChildren, toDelete);
  iter = children.begin();
  for(; iter!=end; ++iter) {
    (*iter)->accept(visit);
  }
  children = newChildren;
  for(expression_node* node : toDelete){
    delete node;
  }
  return this;
}

expression_node* polyadic_multiplication_operator_node::make_pre_canonical() {
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* newChild = (*iter)->make_pre_canonical();
    if(newChild != *iter)
      delete *iter;
    *iter = newChild;
  }
  
  std::list<expression_node*> newChildren;
  std::list<expression_node*> toDelete;
  pre_canonical_multiplication_operator_visitor visit(newChildren, toDelete);
  iter = children.begin();
  end = children.end();
  /*We add nodes to the newChildren list*/
  for(; iter != end; ++iter){
    (*iter)->accept(visit);
  }
  children = newChildren;
  /*We delete nodes specified as deletable*/
  iter = toDelete.begin();
  end = toDelete.end();
  for(; iter != end; ++iter) {
    delete *iter;
  }
  
  /*
   * Now we select all nodes but the last plus operator node if it exists
   * from the children. 
   */
 
  iter = children.begin();
  end = children.end();
  polyadic_addition_operator_node* addNode = nullptr;
  pre_canonical_multiplication_operator_identifier_visitor visit_identifier(addNode);
  for(; iter != end; ++iter){
    (*iter)->accept(visit_identifier);
    
    if(addNode) {
      break;
    }
  }
  /*In the case that there is no addition node, we are done*/
  if(!addNode) return this;
  
  iter = children.begin();
  end = children.end();
  std::list<expression_node*> factors;
  for(; iter != end; ++iter) {
    
    if(*iter != addNode) {
      factors.push_back(*iter);
    }
  }
  
  /*Now we construct and return a polyadic addition node
   in canonical form*/
  std::list<expression_node*> returnTerms;
  iter = addNode->children.begin();
  end = addNode->children.end();
  for(; iter != end; ++iter) {
    std::list<expression_node*> returnFactors;
    
    for(expression_node* i : factors) {
      returnFactors.push_back(i->copy());
    }
    returnFactors.push_back(*iter);
    returnTerms.push_back(
    new polyadic_multiplication_operator_node(returnFactors));
  }
  /*We must now delete children of the add node which were not copied
   Everything else was copied so it will be deleted by the caller.
   Thats why we don't clear this->children*/
  addNode->children.clear();
  
  expression_node* oldNode = (new polyadic_addition_operator_node(returnTerms));
  expression_node* newNode = oldNode->make_pre_canonical();
  if(oldNode != newNode)
    delete oldNode;
  return newNode;
}

