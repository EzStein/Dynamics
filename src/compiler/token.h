#ifndef TOKEN_GUARD
#define TOKEN_GUARD
#include <string>

enum class token {
  ID,
  LEFT_PAREN,
  RIGHT_PAREN,
  END,
  EXCLAMATION,
  CARET,
  UNARY_MINUS,
  ASTERISK,
  FORWARD_SLASH,
  FUNCTION,
  PLUS,
  MINUS,
  ERROR,

  /*Symbols for precedence parsing*/
  NONTERMINAL
};

std::string token_to_string(token tok);
std::ostream& operator<<(std::ostream& out, token tok);

#endif
