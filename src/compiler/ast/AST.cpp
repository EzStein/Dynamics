#include <new>
#include <algorithm>
#include "AST.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/compiler_data.h"

/*Constructs this syntax tree with the given root.
The tree can be constructed from calls to the provided static methods. Once the
AST is built using this constructor, it takes ownership over the tree and becomes responsible
for destroying it*/
AST::AST(expression_node* _root) : root(_root) { }

/*Recursively destroyes the tree*/
AST::~AST() {
  delete root;
}

AST::AST(const AST& ast) : root(ast.root->copy()) { }

AST& AST::operator=(const AST& ast) {
  if(&ast == this) return *this;

  /*Delete the old tree*/
  delete root;
  /*And copy the new one*/
  root = ast.root->copy();
  return *this;
}

AST::AST(AST&& ast) : root(ast.root) {
  /*Transfers ownership from ast to *this by setting ast.root to null thus preventing its immanent r-value deletion*/
  ast.root = nullptr;
}

AST& AST::operator=(AST&& ast) {
  if(&ast == this) return *this;
  delete root;
  root = ast.root;
  ast.root = nullptr;
  return *this;
}

/*Conversion to double,
used for matrix use*/
AST::operator double() {
  if(root->evaluatable()) {
    return root->evaluate();
  } else {
    return 1;
  }
}

/*Conversion from int, used for matrix*/
AST::AST(int val) : root(new integer_number_leaf_node(val)) {
}


AST& AST::simplify() {
  expression_node* newRoot = root->transform_operators();
  if(newRoot != root)
    delete root;
  root = newRoot;

  expression_node* oldRoot = nullptr;
  do {
    delete oldRoot;

    oldRoot = root->copy();

    newRoot = root->level_operators();
    if(newRoot != root)
      delete root;
    root = newRoot;

    newRoot = root->make_pre_canonical();
    if(newRoot != root)
      delete root;
    root = newRoot;

    root->sort();

    newRoot = root->collect_terms();
    if(newRoot != root)
      delete root;
    root = newRoot;

    optimize();
  } while(*oldRoot != *root);
  delete oldRoot;
  return *this;
}

AST& AST::differentiate(const std::string& var) {
  /*We first perform enough tree manipulations to get only the appropriate operators*/
  expression_node* newRoot = root->transform_operators();
  if(newRoot != root)
    delete root;
  root = newRoot;
  newRoot = root->level_operators();
  if(newRoot != root)
    delete root;
  root = newRoot;

  /*Then we perform the differentiation*/
  newRoot = root->differentiate(var);
  if(newRoot != root)
    delete root;
  root = newRoot;

  /*We now need to level operators again, but no transformation*/
  newRoot = root->level_operators();
  if(newRoot != root)
    delete root;
  root = newRoot;
  newRoot = root->make_pre_canonical();
  if(newRoot != root)
    delete root;
  root = newRoot;
  root->sort();
  newRoot = root->collect_terms();
  if(newRoot != root)
    delete root;
  root = newRoot;

  optimize();
  return *this;
}


AST& AST::optimize() {

  expression_node* rootCopy = root->copy();

  expression_node* tmp = rootCopy->optimization_round();


  if(tmp != rootCopy)
    delete rootCopy;
  rootCopy = tmp;

  while(*rootCopy != *root) {
    delete root;
    root = rootCopy;

    rootCopy = root->copy();
    expression_node* tmp = rootCopy->optimization_round();
    if(tmp != rootCopy)
      delete rootCopy;
    rootCopy = tmp;
  }
  delete rootCopy;
  return *this;
}



expression_node* AST::make_variable_leaf_node(symbol::ptr_type symPtr) {
  return new variable_leaf_node(symPtr);
}

expression_node* AST::make_number_leaf_node(double val) {
  return new number_leaf_node(val);
}

expression_node* AST::make_integer_number_leaf_node(long val) {
  return new integer_number_leaf_node(val);
}

template<class NODE_TYPE>
expression_node* AST::make_binary_operator_node(expression_node* leftChild, expression_node* rightChild) {
  return new NODE_TYPE(leftChild, rightChild);
}

template<class NODE_TYPE>
expression_node* AST::make_unary_operator_node(expression_node* child) {
  return new NODE_TYPE(child);
}

template expression_node* AST::make_binary_operator_node<exponentiation_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<binary_addition_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<binary_subtraction_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<binary_multiplication_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_binary_operator_node<division_operator_node>(expression_node*,expression_node*);
template expression_node* AST::make_unary_operator_node<unary_minus_operator_node>(expression_node*);

double AST::evaluate() const {
  return root->evaluate();
}

std::string AST::emit_code_amd64() const {
  compiler_data data(0);
  std::string retVal =
  "pushq %rbp\n"
  "movq %rsp, %rbp\n"
  /*Allocate 16 bytes on the stack. The 8 bytes starting at of -0x08(%rbp)
  holds the mxcsr reg, the old and the new fpu cw regs
  The 8 bytes starting at -0x10(%rbp) are used for scratch space*/
  "subq $0x10, %rsp\n"
  "stmxcsr -0x4(%rbp)\n"
  "fstcw -0x6(%rbp)\n"
  "movw $0x0f7f, -0x8(%rbp)\n"
  "pushq %rbx\n"
  "pushq %r12\n"
  "pushq %r13\n"
  "pushq %r14\n"
  "pushq %r15\n"
  "finit\n"
  "fldcw -0x8(%rbp)\n";

  root->emit_code_amd64(retVal, data);
  retVal +=
  "fstpl -0x10(%rbp)\n"
  "movq -0x10(%rbp), %xmm0\n"
  "popq %r15\n"
  "popq %r14\n"
  "popq %r13\n"
  "popq %r12\n"
  "popq %rbx\n"
  "fldcw -0x6(%rbp)\n"
  "ldmxcsr -0x4(%rbp)\n"
  "movq %rbp, %rsp\n"
  "popq %rbp\n"
  "retq";
  std::cout << retVal << std::endl;
  return retVal;
}

void AST::emit_stack_dec_amd64(std::string& str, compiler_data& data) {

  if(data.stackSizeFPU >= 9) {
    str +=
    "fldl (%rsp)\n"
    "addq $8, %rsp\n"
    "fincstp\n";
  }
  --data.stackSizeFPU;
}

void AST::emit_stack_inc_amd64(std::string& str, compiler_data& data) {

  if(data.stackSizeFPU >= 8) {
    str +=
    "fdecstp\n"
    "subq $8, %rsp\n"
    "fstpl (%rsp)\n";
  }

  /*Increment the size of the stack to reflected the added value*/
  ++data.stackSizeFPU;
}

AST& AST::operator+=(const AST& ast) {
  root = new binary_addition_operator_node(root, ast.root->copy());
  return *this;
}

AST& AST::operator-=(const AST& ast) {
  root = new binary_subtraction_operator_node(root, ast.root->copy());
  return *this;
}

AST& AST::operator*=(const AST& ast) {
  root = new binary_multiplication_operator_node(root, ast.root->copy());
  return *this;
}

AST& AST::operator/=(const AST& ast) {
  root = new division_operator_node(root, ast.root->copy());

  return *this;
}

AST& AST::operator-() {
  root = new unary_minus_operator_node(root);
  return *this;
}

AST operator+(AST lhs, const AST& rhs) {
  return lhs+=rhs;
}

AST operator-(AST lhs, const AST& rhs) {
  return lhs-=rhs;
}

AST operator*(AST lhs, const AST& rhs) {
  return lhs*=rhs;
}

AST operator/(AST lhs, const AST& rhs) {
  return lhs /= rhs;
}

/*
* Called ONLY when neither input can mutate.
*/
/*AST operator*(const AST& lhs, const AST& rhs) {
  return AST(new binary_multiplication_operator_node(lhs.root->copy(), rhs.root->copy()));
}*/


std::ostream& operator<<(std::ostream& out, const AST& ast) {
  ast.root->print(out);
  return out;
}

bool operator==(const AST& ast1, const AST& ast2) {
  return *ast1.root == *ast2.root;
}

bool operator!=(const AST& ast1, const AST& ast2) {
  return *ast1.root != *ast2.root;
}

bool operator<(const AST& ast1, const AST& ast2) {
  return *ast1.root < *ast2.root;
}

bool operator<=(const AST& ast1, const AST& ast2) {
  return *ast1.root <= *ast2.root;
}

bool operator>(const AST& ast1, const AST& ast2) {
  return *ast1.root > *ast2.root;
}

bool operator>=(const AST& ast1, const AST& ast2) {
  return *ast1.root >= *ast2.root;
}
