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

void AST::simplify() {
  expression_node* newRoot = root->transform_negation();
  if(newRoot != root)
    delete root;
  root = newRoot;
}


expression_node* AST::make_variable_leaf_node(symbol::ptr_type symPtr) {
  return new variable_leaf_node(symPtr);
}

expression_node* AST::make_number_leaf_node(double val) {
  return new number_leaf_node(val);
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

std::ostream& AST::emit_code_amd64(std::ostream& acc, unsigned char * buf) const {
  compiler_data data{-1, 0, buf};

  data.executableBuf[++data.offset] = '\x55';
  acc << "push %rbp\n";

  data.executableBuf[++data.offset] = '\x48';
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\xe5';
  acc << "mov %rsp, %rbp\n";

  data.executableBuf[++data.offset] = '\x48';
  data.executableBuf[++data.offset] = '\x83';
  data.executableBuf[++data.offset] = '\xec';
  data.executableBuf[++data.offset] = '\x50';
  acc << "sub $0x50, %rsp\n";

  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\xae';
  data.executableBuf[++data.offset] = '\x5d';
  data.executableBuf[++data.offset] = '\xfc';
  acc << "stmxcsr -0x4(%rbp)\n";

  data.executableBuf[++data.offset] = '\x9b';
  data.executableBuf[++data.offset] = '\xd9';
  data.executableBuf[++data.offset] = '\x7d';
  data.executableBuf[++data.offset] = '\xfa';
  acc << "fstcw -0x6(%rbp)\n";


  data.executableBuf[++data.offset] = '\x66';
  data.executableBuf[++data.offset] = '\xc7';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xf8';
  data.executableBuf[++data.offset] = '\x7f';
  data.executableBuf[++data.offset] = '\x0f';
  acc << "movw $0x0f7f, -0x8(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xb8';
  acc << "movsd %xmm0, -0x48(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x4d';
  data.executableBuf[++data.offset] = '\xc0';
  acc << "movsd %xmm1, -0x40(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x55';
  data.executableBuf[++data.offset] = '\xc8';
  acc << "movsd %xmm2, -0x38(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x5d';
  data.executableBuf[++data.offset] = '\xd0';
  acc << "movsd %xmm3, -0x30(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x65';
  data.executableBuf[++data.offset] = '\xd8';
  acc << "movsd %xmm4, -0x28(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x6d';
  data.executableBuf[++data.offset] = '\xe0';
  acc << "movsd %xmm5, -0x20(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x75';
  data.executableBuf[++data.offset] = '\xe8';
  acc << "movsd %xmm6, -0x18(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x11';
  data.executableBuf[++data.offset] = '\x7d';
  data.executableBuf[++data.offset] = '\xf0';
  acc << "movsd %xmm7, -0x10(%rbp)\n";

  data.executableBuf[++data.offset] = '\x53';
  acc << "push %rbx\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x54';
  acc << "push %r12\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x55';
  acc << "push %r13\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x56';
  acc << "push %r14\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x57';
  acc << "push %r15\n";

  data.executableBuf[++data.offset] = '\x9b';
  data.executableBuf[++data.offset] = '\xdb';
  data.executableBuf[++data.offset] = '\xe3';
  acc << "finit\n";

  data.executableBuf[++data.offset] = '\xd9';
  data.executableBuf[++data.offset] = '\x6d';
  data.executableBuf[++data.offset] = '\xf8';
  acc << "fldcw -0x8(%rbp)\n";
  root->emit_code_amd64(acc, data);

  data.executableBuf[++data.offset] = '\xdd';
  data.executableBuf[++data.offset] = '\x5d';
  data.executableBuf[++data.offset] = '\xf0';
  acc << "fstpl -0x10(%rbp)\n";

  data.executableBuf[++data.offset] = '\xf2';
  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x10';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xf0';
  acc << "movsd -0x10(%rbp), %xmm0\n";

  /*data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\x0e';
  acc << "femms";*/

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x5f';
  acc << "pop %r15\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x5e';
  acc << "pop %r14\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x5d';
  acc << "pop %r13\n";

  data.executableBuf[++data.offset] = '\x41';
  data.executableBuf[++data.offset] = '\x5c';
  acc << "pop %r12\n";

  data.executableBuf[++data.offset] = '\x5b';
  acc << "pop %rbx\n";

  data.executableBuf[++data.offset] = '\xd9';
  data.executableBuf[++data.offset] = '\x6d';
  data.executableBuf[++data.offset] = '\xfa';
  acc << "fldcw -0x6(%rbp)\n";

  data.executableBuf[++data.offset] = '\x0f';
  data.executableBuf[++data.offset] = '\xae';
  data.executableBuf[++data.offset] = '\x55';
  data.executableBuf[++data.offset] = '\xfc';
  acc << "ldmxcsr -0x4(%rbp)\n";

  data.executableBuf[++data.offset] = '\x48';
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\xec';
  acc << "mov %rbp, %rsp\n";

  data.executableBuf[++data.offset] = '\x5d';
  acc << "pop %rbp\n";

  data.executableBuf[++data.offset] = '\xc3';
  acc << "retq\n";
  return acc;
}

std::ostream& AST::emit_code_ia32(std::ostream& acc, unsigned char * buf) const {
  compiler_data data{-1, 0, buf};
  acc << "pushl %ebp\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF5';

  acc << "movl %esp, %ebp\n";
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\xE5';

  acc << "subl $8, %esp\n";
  data.executableBuf[++data.offset] = '\x81';
  data.executableBuf[++data.offset] = '\xEC';
  data.executableBuf[++data.offset] = '\x08';
  data.executableBuf[++data.offset] = '\x00';
  data.executableBuf[++data.offset] = '\x00';
  data.executableBuf[++data.offset] = '\x00';

  acc << "fstcw -2(%ebp)\n";
  data.executableBuf[++data.offset] = '\x9B';
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\x7D';
  data.executableBuf[++data.offset] = '\xFE';

  acc << "stmxcsr -6(%ebp)\n";
  data.executableBuf[++data.offset] = '\x0F';
  data.executableBuf[++data.offset] = '\xAE';
  data.executableBuf[++data.offset] = '\x5D';
  data.executableBuf[++data.offset] = '\xFA';

  /*This rounds DOWN. maybe we should round to nearest!
  But it might have something to do with exponentiation*/
  acc << "movw $0x0f7f, -8(%ebp)\n";
  data.executableBuf[++data.offset] = '\x66';
  data.executableBuf[++data.offset] = '\xC7';
  data.executableBuf[++data.offset] = '\x45';
  data.executableBuf[++data.offset] = '\xF8';
  data.executableBuf[++data.offset] = '\x7F';
  data.executableBuf[++data.offset] = '\x0F';

  acc << "pushl %ebx\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF3';

  acc << "pushl %esi\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF6';

  acc << "pushl %edi\n";
  data.executableBuf[++data.offset] = '\xFF';
  data.executableBuf[++data.offset] = '\xF7';

  acc << "finit\n";
  data.executableBuf[++data.offset] = '\x9B';
  data.executableBuf[++data.offset] = '\xDB';
  data.executableBuf[++data.offset] = '\xE3';

  acc << "fldcw -8(%ebp)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\x6D';
  data.executableBuf[++data.offset] = '\xF8';

  root->emit_code_ia32(acc, data);

  acc << "popl %edi\n";
  data.executableBuf[++data.offset] = '\x5F';

  acc << "popl %esi\n";
  data.executableBuf[++data.offset] = '\x5E';

  acc << "popl %ebx\n";
  data.executableBuf[++data.offset] = '\x5B';

  acc << "fldcw -2(%ebp)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\x6D';
  data.executableBuf[++data.offset] = '\xFE';

  acc << "ldmxcsr -6(%ebp)\n";
  data.executableBuf[++data.offset] = '\x0F';
  data.executableBuf[++data.offset] = '\xAE';
  data.executableBuf[++data.offset] = '\x55';
  data.executableBuf[++data.offset] = '\xFA';

  acc << "movl %ebp, %esp\n";
  data.executableBuf[++data.offset] = '\x89';
  data.executableBuf[++data.offset] = '\xEC';

  acc << "popl %ebp\n";
  data.executableBuf[++data.offset] = '\x5D';

  acc << "ret\n";
  data.executableBuf[++data.offset] = '\xC3';

  return acc;
}

void AST::emit_stack_dec_amd64(std::ostream& acc,  compiler_data& data) {
  if(data.stackSizeFPU >= 9) {
    acc << "fldt (%rsp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x2C';
    data.executableBuf[++data.offset] = '\x24';

    acc << "addq $10, %rsp\n";
    data.executableBuf[++data.offset] = '\x48';
    data.executableBuf[++data.offset] = '\x83';
    data.executableBuf[++data.offset] = '\xc4';
    data.executableBuf[++data.offset] = '\x0a';

    acc << "fincstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF7';
  }
  --data.stackSizeFPU;
}

void AST::emit_stack_inc_amd64(std::ostream& acc, compiler_data& data) {
  if(data.stackSizeFPU >= 8) {
    acc << "fdecstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF6';

    /*Make room for the extended double precision data on the stack*/
    acc << "subq $10, %rsp\n";
    data.executableBuf[++data.offset] = '\x48';
    data.executableBuf[++data.offset] = '\x83';
    data.executableBuf[++data.offset] = '\xec';
    data.executableBuf[++data.offset] = '\x0a';

    acc << "fstpt (%rsp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x3C';
    data.executableBuf[++data.offset] = '\x24';
  }

  /*Increment the size of the stack to reflected the added value*/
  ++data.stackSizeFPU;
}

unsigned int AST::code_size() const {
  return root->code_size() + 50;
}

std::ostream& operator<<(std::ostream& out, const AST& ast) {
  ast.root->print(out);
  return out;
}
