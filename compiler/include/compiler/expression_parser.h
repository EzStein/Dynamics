#ifndef DYNSOLVER_COMPILER_EXPRESSION_PARSER_H_
#define DYNSOLVER_COMPILER_EXPRESSION_PARSER_H_

#include <string>

#include "compiler/ast/AST.h"
#include "compiler/ast/expression_node.h"
#include "parser/grammar.h"
#include "parser/lr_parser.h"
#include "regex/lexer.h"
#include "compiler/ir/symbol.h"

namespace dynsolver {
namespace compiler {

// This class is used to parse expressions. It uses a the grammar class/parser generator
// to build an lr_parser for the particular grammar. We build an abstract syntax
// from the grammar.
// The grammar is stated below. Each word is its own token except for
// epsilon which is the empty string.
//
// EXPRESSION -> EXPRESSION PLUS TERM
// EXPRESSION -> EXPRESSION MINUS TERM
// EXPRESSION -> TERM
// TERM -> TERM TIMES FACTOR
// TERM -> TERM DIVIDE FACTOR
// TERM -> FACTOR
// FACTOR -> BASE CARET FACTOR
// FACTOR -> MINUS FACTOR
// FACTOR -> BASE
// BASE -> FUNCTION LEFT_PARENTHESIS PARAMETER_LIST RIGHT_PARENTHESIS
// BASE -> LEFT_PARENTHESIS EXPRESSION RIGHT_PARENTHESIS
// BASE -> NUMBER
// BASE -> CONSTANT
// BASE -> VARIABLE
// PARAMETER_LIST -> EPSILON
// PARAMETER_LIST -> NONEMPTY_PARAMETER_LIST
// NONEMPTY_PARAMETER_LIST -> EXPRESSION COMMA NONEMPTY_PARAMETER_LIST
// NONEMPTY_PARAMETER_LIST -> EXPRESSION
//
// In this grammar, exponentiation has highest precedence and is right
// associative. Unary minus is next in precedence and is also right
// associative. Multiplication and division come next and they are left
// associative. Addition and subtraction have lowest precedence and are
// left associative.
//
// Currently, all constants are words entirely in upper case. Variables
// are any letter, number and underscore combinations 
class expression_parser {
 public:
  // Default constructor creates the lexer and the grammar from which the
  // underlying lr_parser is generated. In general, the parser should only be
  // constructed once, and can then be used to parse multiple inputs.
  expression_parser();

  // Parses the provided expression, throwing an exception if the input is not
  // a legal expression according to the grammar above. Returns an abstract
  // syntax tree representing the input.
  AST parse(const std::string& input, const std::list<::symbol>& symbolTable);

 private:
  regex::lexer lexer;
  parser::lr_parser parser;
  const std::list<::symbol>* symbols;

  // The following static variables represent tokens of input.
  static const int EXPRESSION = 0;
  static const int TERM = 1;
  static const int FACTOR = 2;
  static const int BASE = 3;
  static const int PARAMETER_LIST = 4;
  static const int NONEMPTY_PARAMETER_LIST = 5;
  static const int PLUS = 6;
  static const int MINUS = 7;
  static const int TIMES = 8;
  static const int DIVIDE = 9;
  static const int CARET = 10;
  static const int LEFT_PARENTHESIS = 11;
  static const int RIGHT_PARENTHESIS = 12;
  static const int COMMA = 13;
  static const int NUMBER = 14;
  static const int VARIABLE = 15;
  static const int CONSTANT = 16;
  static const int FUNCTION = 17;

 public:
  // This is the call back function used when parsing the input and building
  // the abstract syntax tree.
  expression_node* operator()(int nonterminal, int productionIndex,
                            const std::list<expression_node*>& children,
                            const std::list<std::string>& lexeme);
};

} // namespace compiler
} // namespace parser

#endif
