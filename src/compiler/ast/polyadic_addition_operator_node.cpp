#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/visitor/mutation/level_addition_operator_visitor.h"
#include "compiler/ast/visitor/mutation/pre_canonical_addition_operator_visitor.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "polyadic_multiplication_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/integer_number_leaf_node.h"

#include "number_leaf_node.h"
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

long polyadic_addition_operator_node::evaluate_as_integer() const {
  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  long acc = 0;
  for(; iter != end; ++iter) {
    acc += (*iter)->evaluate_as_integer();
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

void polyadic_addition_operator_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  if(children.size() == 1) {
    /*There is only one child so its value is emitted onto the stack*/
    (*children.begin())->emit_code_amd64(acc, data);
    return;
  }

  const_iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  /*We first emit the first child onto the stack.*/
  (*iter)->emit_code_amd64(acc, data);
  ++iter;

  for(; iter != end; ++iter) {
    /*Guarenteed to execute at least once*/
    /*We put the next child onto the stack*/
    (*iter)->emit_code_amd64(acc, data);
    acc += "faddp %st(0), %st(1)\n";
    AST::emit_stack_dec_amd64(acc, data);
  }
  /*At the end, %st0 contains the sum so we don't need to do anything*/
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

void polyadic_addition_operator_node::accept(visitor* v) {
  v->visit(this);
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
    (*iter)->accept(&visit);
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
    (*iter)->accept(&visit);
  }
  children = newChildren;
  for(expression_node* node : toDelete){
    delete node;
  }
  return this;
}

expression_node* polyadic_addition_operator_node::collect_terms() {
  /*We first call the super method which recursively calls collect_terms on the children*/
  polyadic_operator_node::collect_terms();
  /*We may assume that all the children have collected terms and are in precanonical form
   and are all sorted
   We are still in precanonical form because we guarantee that a call to collect_terms
   will not change a polyadic type. We may not be sorted however!, so we must resort ourselves*/
  this->sort();

  /*Evaluatable nodes are at the front. We create
   a new list to hold the new children*/
  std::list<expression_node*> newChildren;
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    if(!(*iter)->evaluatable())
      break;
    newChildren.push_back(*iter);
  }
  /*If we reached the end then all nodes are evaluatable so we have already collected terms*/
  if(iter == end)
    return this;

  /*Otherwise iter points to the first nonevaluatable node.
   This node is a sorted, precanonical, collected terms polyadic multiplication operator node*/
  /*We first construct a new polyadic multiplication operator node that contains
   only the nonevaluatable children of the node we call this the non_coefficient*/
  while(iter != end) {
    polyadic_multiplication_operator_node* node = dynamic_cast<polyadic_multiplication_operator_node*>(*iter);
    std::list<expression_node*> coefficient_sum, non_evaluatable_children;
    iterator_t termIter = node->children.begin();
    const_iterator_t termEnd = node->children.end();
    for(; termIter != termEnd; ++termIter) {
      if(!(*termIter)->evaluatable()) {
        non_evaluatable_children.push_back((*termIter)->copy());
      }
    }
    polyadic_multiplication_operator_node* non_coefficient
        = new polyadic_multiplication_operator_node(non_evaluatable_children);
    non_evaluatable_children.clear();

    /*Now we iterate over terms until we get to one with a different non_coefficient*/
    for(; iter != end; ++iter) {
      node = dynamic_cast<polyadic_multiplication_operator_node*>(*iter);
      /*We collect the evaluatable and non_evaluatable children as before*/
      std::list<expression_node*> evaluatable_children;
      termIter = node->children.begin();
      termEnd = node->children.end();
      for(; termIter != termEnd; ++termIter) {
        if(!(*termIter)->evaluatable()) {
          non_evaluatable_children.push_back(*termIter);
        } else {
          evaluatable_children.push_back(*termIter);
        }
      }
      polyadic_multiplication_operator_node* non_evaluatable_part
        = new polyadic_multiplication_operator_node(non_evaluatable_children);
      non_evaluatable_children.clear();
      /*If the terms have the same nonevaluatable part*/
      if(*non_evaluatable_part == *non_coefficient) {
        coefficient_sum.push_back(new polyadic_multiplication_operator_node(evaluatable_children));
      } else
        break;

      /*Now we delete the node which will no longer be used. However,
       we don't want to delete the evaluatable part because it has been added
       to a coefficient_sum, we just delete the non-evaluatable part*/
      delete non_evaluatable_part;
      /*We also delete the node but not necesarily its children*/
      node->children.clear();
      delete node;
    }
    expression_node* tmp = new polyadic_addition_operator_node(coefficient_sum);
    expression_node* coefficient;
    if(!tmp->evaluatable()) throw "THIS SHOULD NOT OCCUR";
    if(tmp->is_integral())
      coefficient = new integer_number_leaf_node(tmp->evaluate_as_integer());
    else
      coefficient = new number_leaf_node(tmp->evaluate());
    /*We now no longer need tmp or the coefficient sum, so we delete it*/
    delete tmp;

    /*Finally, we add the product of the coefficient and the non_coefficient to the list of new children*/
    non_coefficient->children.push_front(
    new exponentiation_operator_node(coefficient, new integer_number_leaf_node(1)));
    newChildren.push_back(non_coefficient);
  }
  /*Note, this node will be deleted by the caller, but we have already deleted any children
   if they need to be deleted, so we clear the children*/
  children.clear();
  /*Now observe that this node is necesarily in canonical for because it is a sum,
   of products of nodes which are exponentiation nodes in canonical form*/
  return new polyadic_addition_operator_node(newChildren);

}

expression_node* polyadic_addition_operator_node::optimization_round() {
  polyadic_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate_as_integer());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  /*We first remove all children which evaluate to zero*/
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  while(iter != end) {
    if((*iter)->evaluatable() && (*iter)->is_integral() && (*iter)->evaluate_as_integer() == 0) {
      /*We delete the node and then remove it from the list*/
      delete *iter;
      iter = children.erase(iter);
      /*Iter points to the element after the one that was erased, so we
       don't need to increment it. Also the end iterator is not invalidated.*/
    } else {
      ++iter;
    }
  }

  if(children.size() == 1) {
    expression_node* tmp = *(children.begin());
    /*We clear the list so that the returned child isn't deleted*/
    children.clear();
    return tmp;
  } else if(children.size() == 0) {
    /*If there are no children we default to returning zero*/
    return new integer_number_leaf_node(0);
  } else {
    return this;
  }
}

expression_node* polyadic_addition_operator_node::differentiate(const std::string& var) {
  /*We simply take the derivative of each child*/
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  for(; iter != end; ++iter) {
    expression_node* tmp = (*iter)->differentiate(var);
    if(tmp != *iter)
      delete *iter;
    (*iter) = tmp;
  }
  return this;
}
