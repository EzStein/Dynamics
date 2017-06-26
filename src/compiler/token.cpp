#include "token.h"
#include <iostream>

std::string token_to_string(token tok) {
  switch(tok) {
    case token::ID:
    return "ID";
    case token::ENDPOINT:
    return "ENDPOINT";
    case token::EXCLAMATION:
    return "EXCLAMATION";
    case token::CARET:
    return "CARET";
    case token::UNARY_MINUS:
    return "UNARY_MINUS";
    case token::ASTERISK:
    return "ASTERISK";
    case token::FORWARD_SLASH:
    return "FORWARD_SLASH";
    case token::FUNCTION:
    return "FUNCTION";
    case token::PLUS:
    return "PLUS";
    case token::MINUS:
    return "MINUS";
    case token::ERROR:
    return "ERROR";
    case token::LEFT_PAREN:
    return "LEFT_PAREN";
    case token::RIGHT_PAREN:
    return "RIGHT_PAREN";
    case token::NONTERMINAL:
    return "NONTERMINAL";
    case token::NUMBER:
    return "NUMBER";
    case token::TOKEN_COUNT:
    return "TOKEN_COUNT";
  }
  return "NO_SUCH_TOKEN";
}
std::ostream& operator<<(std::ostream& out, token tok) {
  out << token_to_string(tok);
  return out;
}
