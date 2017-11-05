#ifndef AST_GUARD
#define AST_GUARD
#include <cstddef>
#include <iostream>
#include "compiler/ast/expression_node.h"
#include "compiler/ast/binary_addition_operator_node.h"
#include "compiler/ast/binary_multiplication_operator_node.h"
#include "compiler/ast/binary_subtraction_operator_node.h"
#include "compiler/ast/division_operator_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"

class AST {
public:
  explicit AST(expression_node*);

  /*Destroys the AST*/
  ~AST();

  AST() : root(nullptr) { };

  explicit AST(const std::string& str);

  /*Copy constructor. Performs a deep copy of the tree*/
  AST(const AST&);

  /*Move constructor*/
  AST(AST&&);

  /*Assignment operator which performs a deep copy*/
  AST& operator=(const AST&);

  /*Move assignment*/
  AST& operator=(AST&&);

  AST& operator+=(const AST&);
  AST& operator-=(const AST&);
  AST& operator*=(const AST&);
  AST& operator/=(const AST&);
  AST& operator-();
  friend bool operator==(const AST&, const AST&);
  friend bool operator!=(const AST&, const AST&);
  friend bool operator<(const AST&, const AST&);
  friend bool operator<=(const AST&, const AST&);
  friend bool operator>(const AST&, const AST&);
  friend bool operator>=(const AST&, const AST&);
  //friend AST operator*(const AST& lhs, const AST& rhs);
  friend AST operator*(AST lhs, const AST& rhs);
  friend AST operator+(AST lhs, const AST& rhs);
  friend AST operator-(AST lhs, const AST& rhs);
  friend AST operator/(AST lhs, const AST& rhs);


  /*Conversion to double,
  used for matrix use*/
  explicit operator double();

  /*Conversion from int, used for matrix*/
  explicit AST(int);

  /*
  * Returns a reference to itself
  */
  AST& simplify();

  AST& differentiate(const std::string&);
  /*Constructs the binary operator of the appropriate type whose children are given by the provided pointers.
  * The function returns a pointer to the constructed node*/
  template<class NODE_TYPE>
  static expression_node* make_binary_operator_node(expression_node* leftChild, expression_node* rightChild);

  /*Constructs the unary operator of the appropriate type whose child is given by the provided pointer.
  * The function returns a pointer to the constructed node*/
  template<class NODE_TYPE>
  static expression_node* make_unary_operator_node(expression_node* child);

  static expression_node* make_variable_leaf_node(symbol::ptr_type);
  static expression_node* make_number_leaf_node(double val);
  static expression_node* make_integer_number_leaf_node(long val);

  double evaluate() const;

  /*
   * Calls optimization_round on root until it is unchanged.
   */
  AST& optimize();

  std::string emit_code_amd64() const;

  /*
  * This should be called immediatly after on operation is performed that decrements the FPU stack.
  * The function will emmit code to push a stored register value onto the bottom of the stack.
  */
  static void emit_stack_dec_amd64(std::string&, compiler_data& data);
  /*
  * This should be called immediatly before an operation that increments the stack.
  * It will potentially store the bottom of the stack in memory.
  */
  static void emit_stack_inc_amd64(std::string&, compiler_data& data);

  friend std::ostream& operator<<(std::ostream&, const AST&);
  friend bool operator==(AST&, AST&);
private:
  expression_node * root;
};

std::ostream& operator<<(std::ostream&, const AST&);
bool operator==(AST&, AST&);

#endif
