/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <list>
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "expression_node.h"
#include "compiler/ast/visitor/mutation/level_multiplication_operator_visitor.h"
#include "compiler/ast/visitor/mutation/pre_canonical_multiplication_operator_visitor.h"
#include "compiler/ast/visitor/mutation/pre_canonical_multiplication_operator_identifier_visitor.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/binary_operator_node.h"



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

void polyadic_multiplication_operator_node::accept(visitor* v) {
  v->visit(this);
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
    (*iter)->accept(&visit);
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
    (*iter)->accept(&visit);
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
    (*iter)->accept(&visit_identifier);

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

expression_node* polyadic_multiplication_operator_node::collect_terms() {

  /*We first call the super method which recursively calls collect_terms on the children*/
  polyadic_operator_node::collect_terms();
  /*This node
   is in precanonical form because collect_terms is guaranteed to preserve the node type.
   But, we may no longer be sorted*/
  this->sort();
  /* We don't need to combine the evaluatable terms
   Instead we combine any nonevaluatable terms and put them in a newChildren list*/
  std::list<expression_node*> newChildren;
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  /*We first iterate till we get to the first non evaluatable term*/
  for(; iter != end; ++iter) {
    if((*iter)->evaluatable())
      newChildren.push_back(*iter);
    else
      break;
  }
  /*Iter now points to either the end of the list or to the first
   non-evaluatable term. If it points to the end, we are done and no collection is necessary*/
  if(iter == end)
    return this;

  while(iter != end) {
    /*This term is an exponentiation_operator_node.
     * We save the base of this term and iterate until we get to the first base that
     does not equal this term. While doing so, we add the exponents*/
    std::list<expression_node*> exponentChildren;
    expression_node* base = dynamic_cast<exponentiation_operator_node*>(*iter)->leftChild->copy();
    for(; iter != end; ++iter) {
      exponentiation_operator_node* node = dynamic_cast<exponentiation_operator_node*>(*iter);
      if(*base == *(node->leftChild)) {
        exponentChildren.push_back(node->rightChild);
        /*We now need to delete the exponentiation node that we just examined,
         however the exponent is going to be used later, so we set the right child to nullptr
         and then delete the operator*/
        node->rightChild = nullptr;
        delete node;
      } else {

        break;
      }
    }
    expression_node* newExponent = new polyadic_addition_operator_node(exponentChildren);
    /*Note this new exponent may not be in precanonical form be sorted or have collected terms*/
    expression_node* tmp = newExponent->make_pre_canonical();
    if(tmp != newExponent)
      delete newExponent;
    newExponent = tmp;
    newExponent->sort();
    tmp = newExponent->collect_terms();
    if(tmp != newExponent)
      delete newExponent;
    newExponent = tmp;

    newChildren.push_back(new exponentiation_operator_node(base, newExponent));

    /*Note iter now equals end or it points to a new exponentiation node with a different base*/
  }

  expression_node* retVal = new polyadic_multiplication_operator_node(newChildren);
  //expression_node* tmp = retVal->make_pre_canonical();

  /*Since we are returning a new value we must set all the current children to null or clear them*/
  children.clear();
  /*Now when the caller deletes this, the children that are now owned by new node
   will not be deleted*/
  return retVal;
}

expression_node* polyadic_multiplication_operator_node::optimization_round() {
  polyadic_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  /*We first remove all children which evaluate to one,
   and if we encounter one that evaluates to zero, we immediatly return a zero integer node*/
  iterator_t iter = children.begin();
  const_iterator_t end = children.end();
  while(iter != end) {

    if((*iter)->evaluatable()) {

      double val = (*iter)->evaluate();
      if(val == 1) {

        /*We delete the node and then remove it from the list*/
        delete *iter;
        iter = children.erase(iter);
        /*Iter points to the element after the one that was erased, so we
         don't need to increment it. Also the end iterator is not invalidated.*/
        std::cout << "A" <<std::endl;
      } else if(val == 0) {

        /*We clear the children and return a zero node*/
        children.clear();
        return new integer_number_leaf_node(0);
      } else {

        ++iter;
      }
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
    /*If there are no children we default to returning one*/
    return new integer_number_leaf_node(1);
  } else {
    return this;
  }
}

expression_node* polyadic_multiplication_operator_node::differentiate(const std::string& var) {
  /*We use the product rule to construct the derivative*/
  const_iterator_t iter1 = children.begin();
  const_iterator_t end = children.end();
  std::list<expression_node*> sum;
  for(; iter1!=end; ++iter1) {
    std::list<expression_node*> terms;
    iterator_t iter2 = children.begin();
    for(; iter2 != end; ++iter2) {
      if(iter2 == iter1){
        expression_node* tmp = (*iter2)->copy();
        expression_node* tmp2 = tmp->differentiate(var);
        if(tmp2 != tmp)
          delete tmp;
        tmp = tmp2;
        terms.push_back(tmp);
      } else {
        terms.push_back((*iter2)->copy());
      }
    }
    sum.push_back(new polyadic_multiplication_operator_node(terms));
  }
  return new polyadic_addition_operator_node(sum);
}
