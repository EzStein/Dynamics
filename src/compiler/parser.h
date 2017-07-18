#ifndef PARSER_GUARD
#define PARSER_GUARD
#include "compiler/ast/AST.h"
#include "compiler/lex/lexer.h"
#include "compiler/lex/token.h"

/*This class represents an operator precedence parser that can parse expressions*/
class parser {
public:
  parser(std::istream&);
  ~parser();

  AST parse();

private:
  lexer lex;
  unsigned char * opArr;
  precedence get_precedence(token tk1, token tk2) const;

  static const unsigned int CHUNK_SIZE = 1 + (static_cast<unsigned int>(token::TOKEN_COUNT) - 1)/4;

  precedence get_precedence(token tk1, token tk2);
  void set_precedence(token tk1, token tk2, precedence prec);
};
#endif
