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
    case token::TOKEN_COUNT:
    return "TOKEN_COUNT";
  }
  return "NO_SUCH_TOKEN";
}
std::ostream& operator<<(std::ostream& out, token tok) {
  out << token_to_string(tok);
  return out;
}

precedence get_precedence(const unsigned char * array, token tk1, token tk2) {
  return static_cast<precedence>((array[static_cast<unsigned int>(tk1) * CHUNK_SIZE
  + static_cast<unsigned int>(tk2)/4] & (0xC0u >> 2*(static_cast<unsigned int>(tk2)%4))) >> 2*(4-(static_cast<unsigned int>(tk2)%4 + 1)));
}

void set_precedence(unsigned char * array, token tk1, token tk2, precedence prec) {
  unsigned char msk = ~(0xC0u >> 2*(static_cast<unsigned int>(tk2)%4));
  unsigned char val = static_cast<unsigned char>(
    static_cast<unsigned int>(prec) << 2*(4 - (static_cast<unsigned int>(tk2)%4 + 1)));

  /*Clear away the bits that were previously there*/
  array[static_cast<unsigned int>(tk1) * CHUNK_SIZE + static_cast<unsigned int>(tk2)/4] &= msk;

  /*Set the appropriate bits*/
  array[static_cast<unsigned int>(tk1) * CHUNK_SIZE + static_cast<unsigned int>(tk2)/4] |= val;
}
