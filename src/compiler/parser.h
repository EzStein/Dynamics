#ifndef PARSER_GUARD
#define PARSER_GUARD
#include "compiler/lexer.h"
#include "compiler/token.h"

/*This class represents an operator precedence parser that can parse expressions*/
class parser {
public:
  parser(std::istream&, const std::map<std::string, token>&, unsigned char const * const opArr);
  void parse();
private:
  lexer lex;
  unsigned char const * const opArr;

  precedence get_precedence(token tk1, token tk2) const;
  //void parser::set_precedence(token tk1, token tk2, precedence prec) const;
};
#endif
