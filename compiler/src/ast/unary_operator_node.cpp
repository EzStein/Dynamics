#include "compiler/ast/unary_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
unary_operator_node::unary_operator_node(expression_node* _child) : child(_child) {

}

unary_operator_node::~unary_operator_node() {
  delete child;
}

bool unary_operator_node::evaluatable() const {
  return child->evaluatable();
}

expression_node* unary_operator_node::transform_operators() {
  expression_node* newChild = child->transform_operators();
  if(newChild != child)
    delete child;
  child = newChild;
  return this;
}

expression_node* unary_operator_node::level_operators() {
  expression_node* newChild = child->level_operators();
  if(newChild != child)
    delete child;
  child = newChild;
  return this;
}

expression_node* unary_operator_node::make_pre_canonical() {
  throw "Attempt to traverse a binary operator during pre-canonical formation";
  return nullptr;
}

void unary_operator_node::sort() {
  child->sort();
}

expression_node* unary_operator_node::collect_terms() {
  throw "UNARY OPERATOR NOT REQUIRED TO IMPLEMENT COLLECT_TERMS";
}

expression_node* unary_operator_node::optimization_round() {
  expression_node* tmp = child->optimization_round();
  if(tmp != child)
    delete child;
  child = tmp;

  return this;
}
