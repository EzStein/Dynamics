#ifndef PARSER_GUARD
#define PARSER_GUARD
#include "compiler/lexer.h"
#include "compiler/token.h"

/*This class represents an operator precedence parser that can parse expressions*/
class parser {
public:
  parser(std::istream&, const std::map<std::string, token>&);
  void parse();
private:
  std::istream& stream;
  const lexer lex;
};
#endif
