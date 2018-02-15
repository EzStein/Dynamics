#include "parser/syntax_exception.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace dynsolver {
namespace parser {

namespace {
// This is a helper function to generate the what message.
std::string generate_message(int unexpectedToken,
                             const std::vector<int>& expectedTokens,
                             int tokenPosition, int characterPosition,
                             int linePosition, const std::string& lexeme) {
  std::string message =
      "Syntax Exception: unexpected token '" + lexeme +
      "' (" + std::to_string(unexpectedToken) +
      ") at position " + std::to_string(characterPosition) +
      " (" + std::to_string(tokenPosition) +
      ") on line " + std::to_string(linePosition) +
      ". Expected tokens:";
  for(std::vector<int>::const_iterator iter = expectedTokens.begin();
      iter != expectedTokens.end(); ++iter) {
    message += " " + std::to_string(*iter) + ",";
  }
  message += ".";
  return message;
}
} // namespace anonymous

syntax_exception::syntax_exception(int unexpectedToken,
                                   const std::vector<int>& expectedTokens,
                                   int tokenPosition, int characterPosition,
                                   int linePosition, const std::string& lexeme)
    : std::runtime_error(generate_message(unexpectedToken, expectedTokens,
                                          tokenPosition, characterPosition,
                                          linePosition, lexeme)),
    unexpectedToken(unexpectedToken),
  expectedTokens(expectedTokens), tokenPosition(tokenPosition),
  characterPosition(characterPosition), linePosition(linePosition),
  unexpectedLexeme(lexeme) { }

int syntax_exception::unexpected_token() const {
  return unexpectedToken;
}

std::vector<int>::const_iterator syntax_exception::expected_tokens_begin() const {
  return expectedTokens.begin();
}

std::vector<int>::const_iterator syntax_exception::expected_tokens_end() const {
  return expectedTokens.end();
}

std::string syntax_exception::unexpected_lexeme() const {
  return unexpectedLexeme;
}

int syntax_exception::token_position() const {
  return tokenPosition;
}

int syntax_exception::character_position() const {
  return characterPosition;
}

int syntax_exception::line_position() const {
  return linePosition;
}
} // namespace parser
} // namespace dynsolver
