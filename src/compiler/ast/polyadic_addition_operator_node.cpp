#include "compiler/ast/polyadic_addition_operator_node.h"

polyadic_addition_operator_node::polyadic_addition_operator_node(expression_node* firstChild) {
  children.push_back(firstChild);
}

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
  for(; iter != end; ++iter) {
    out << '(';
    (*iter)->print(out) << ')';
    if(iter != end - 1)
     out << "+";
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
    val &&= (*iter)->is_integral();
  }
  return val;
}
