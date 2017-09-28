#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/visitor/mutation/level_addition_operator_visitor.h"
#include "compiler/ast/visitor/mutation/pre_canonical_addition_operator_visitor.h"
polyadic_addition_operator_node::polyadic_addition_operator_node(expression_node* firstChild) {
  children.push_back(firstChild);
}
polyadic_addition_operator_node::polyadic_addition_operator_node(
std::list<expression_node*> _children) : polyadic_operator_node(_children) {}

double polyadic_addition_operator_node::evaluate() const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  double acc = 0.0;
  for(; iter != end; ++iter) {
    acc += (*iter)->evaluate();
  }
  return acc;
}

std::ostream& polyadic_addition_operator_node::print(std::ostream& out) const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  out << '(';
  (*iter)->print(out);
  out << ')';
  ++iter;
  for(; iter != end; ++iter) {
    out << "+";
    out << '(';
    (*iter)->print(out) << ')';
  }
  return out;
}

std::ostream& polyadic_addition_operator_node::emit_code_ia32(std::ostream& out, compiler_data&) const {
  return out;
}
std::ostream& polyadic_addition_operator_node::emit_code_amd64(std::ostream& out, compiler_data&) const {
  return out;
}
unsigned int polyadic_addition_operator_node::code_size() const {
  return 100;
}

expression_node* polyadic_addition_operator_node::copy() const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  /*Add the first child copy to the list*/
  polyadic_addition_operator_node* copy
  = new polyadic_addition_operator_node((*iter)->copy());
  ++iter; /*Start on the second child*/
  for(; iter != end; ++iter) {
    copy->children.push_back((*iter)->copy());
  }
  return copy;
}

bool polyadic_addition_operator_node::is_integral() const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  bool val = true;
  for(; iter != end; ++iter) {
    val = val && (*iter)->is_integral();
  }
  return val;
}

void polyadic_addition_operator_node::accept(visitor& v) {
  v.visit(this);
}

expression_node* polyadic_addition_operator_node::level_operators() {
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* newChild = (*iter)->level_operators();
    if(*iter != newChild)
      delete *iter;
    *iter = newChild;
  }
  std::list<expression_node*> newChildren;
  std::list<expression_node*> toDelete;
  level_addition_operator_visitor visit(newChildren, toDelete);
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

expression_node* polyadic_addition_operator_node::make_pre_canonical() {
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* newChild = (*iter)->make_pre_canonical();
    if(*iter != newChild)
      delete *iter;
    *iter = newChild;
  }
  std::list<expression_node*> newChildren, toDelete;
  pre_canonical_addition_operator_visitor visit(newChildren, toDelete);
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
