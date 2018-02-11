#ifndef DYNSOLVER_COMPILER_PARSER_H_
#define DYNSOLVER_COMPILER_PARSER_H_


// The nomenclature found in literature on parsers is somewhat confusing for
// a beginner. Often the same terms mean different things in different texts.
// There are two wide classes of grammers which are often parsed differently.
//
// LL refers to top-down parsing, that is, an algorithm which produces a pre-order
// traversal of the parse tree. Often times LL gramers are parsed using 
// recursive descent or table driven parsing. An LL(k) parser uses k tokens of
// lookahead before determining which production to use for the parse. Top-down
// parsers also may have backtracking capabilities (both recursive descent and
// table driven parsers can use this) but the grammer it parsers may not be
// considered an LL(k) grammar. Thus the following words all refer to the
// same idea: LL(k), top-down, bactracking recursive descent,
// predictive/deterministic recursive descent, top-down table driven,
// pre-order traversal. To construct a table driven LL(1) parser for a grammar
// the grammar must not be left recursive or ambiguous. Additionally,
// the grammar must be left factored. It is often possible to take
// a grammar and form an equivalent grammar which is unambiguous, left-factored,
// and non-left-recursive. It can be shown that LL(1) grammars are of this
// form. Additionally, certain other restrictions apply.
//
// LR refers to bottom-up parsing and produces a post order traversal of the
// parse tree. LR(k) uses k tokens of lookahead. Precedence and operator
// precedence parsers are some types of bottom up parsers. Bottom up parsing
// is commonly called shift-reduce parsing. It functions by reducing the input
// string to the start nonterminal in the grammar. In doing so, it constructs
// a derivation in reverse. When we read the input from left to right,
// we attempt to reduce the leftmost symbols. The derivation that is constructed
// is thus a rightmost derivation.






#include "compiler/ast/AST.h"
#include "compiler/lex/lexer.h"
#include "compiler/lex/token.h"

/*This class represents an operator precedence parser that can parse expressions*/
class parser {
public:
  parser(std::istream&);
  ~parser();

  /*
  * Returns an AST representing the parsed expression.
  * It fills up the symbol table provided with imformation
  * on the symbols and variables involved.
  */
  AST parse(std::list<symbol>&);

private:
  lexer lex;
  unsigned char * opArr;
  precedence get_precedence(token tk1, token tk2) const;

  static const unsigned int CHUNK_SIZE = 1 + (static_cast<unsigned int>(token::TOKEN_COUNT) - 1)/4;

  precedence get_precedence(token tk1, token tk2);
  void set_precedence(token tk1, token tk2, precedence prec);
};
#endif
