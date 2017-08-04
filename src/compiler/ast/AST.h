#ifndef AST_GUARD
#define AST_GUARD
#include <cstddef>
#include <iostream>
#include "compiler/ast/expression_node.h"
#include "compiler/ast/variable_leaf_node.h"
class AST {
public:
  /*Constructs the AST with an estimate on how much memory it will require*/
  explicit AST(size_t);

  /*Constructs this AST with a default memory size*/
  AST();

  /*Destroys the AST*/
  ~AST();

  /*Copy constructor. Performs a deep copy of the tree*/
  AST(const AST&);

  /*Move constructor*/
  AST(AST&&);

  /*Assignment operator which performs a deep copy*/
  AST& operator=(const AST&);

  /*Move assignment*/
  AST& operator=(AST&&);

  /*Constructs the binary operator of the appropriate type whose children are given by the provided pointers.
  * The function returns a pointer to the constructed node*/
  template<class NODE_TYPE>
  expression_node* make_binary_operator_node(expression_node* leftChild, expression_node* rightChild);

  /*Constructs the unary operator of the appropriate type whose child is given by the provided pointer.
  * The function returns a pointer to the constructed node*/
  template<class NODE_TYPE>
  expression_node* make_unary_operator_node(expression_node* child);

  expression_node* make_variable_leaf_node(variable_leaf_node::symbol_ptr_type);
  expression_node* make_number_leaf_node(double val);

  double evaluate() const;

  std::ostream& emit_code(std::ostream&, unsigned char *) const;
  unsigned int code_size() const;

  void set_root(expression_node*);

  void add_symbol(symbol);
  friend std::ostream& operator<<(std::ostream&, const AST&);
private:
  /*The symbol table*/
  std::list<symbol> symbolTable;
  expression_node * root;

  /*Memory chunk used to store all the nodes contiguously. It must be allocated with enough memory initially*/
  unsigned char * mem;

  /*This is an index to the first empty chunk of memory in mem.
  * That is, when a make node function is called, a node will be created
  starting at mem[allocatorOffset]*/
  size_t allocatorOffset;
  size_t memSize;
};

std::ostream& operator<<(std::ostream&, const AST&);


#endif
