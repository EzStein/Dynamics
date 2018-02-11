#ifndef DYNSOLVER_PARSER_SYNTAX_EXCEPTION_H_
#define DYNSOLVER_PARSER_SYNTAX_EXCEPTION_H_

#include <stdexcept>
#include <string>
#include <vector>

namespace dynsolver {
namespace parser {

// This exception is thrown during a parse if the provided input is not
// recognized by the grammar. The most basic error handling a parse can
// provide is to indicate when an unexpected token occurs, what that token is
// and what token(s) it expected.
// If the list of expected tokens is empty then we have finished parsing but
// there is still input left. If on the other hand tokenPosition is
// syntax_exception::kEndPosition then we have reached the end of input
// prematurely.
// We also indentify where in the input the error occured. tokenPosition is
// the number of tokens seen (including the offending token) before the error.
// characterPosition is the number of characters seen (including the first
// character of the offending token) before the error. lexeme indicates
// lexeme of the offending token. linePosition indicates the line number of
// the offending lexeme.
class syntax_exception : public runtime_error {
 public:
  static constexpr kEndPosition(-1);
  
  syntax_exception(int unexpectedToken,
                   std::vector<int>& expectedTokens,
                   int tokenPosition, int characterPosition,
                   int linePosition, const std::string& lexeme);

  // Returns the token that the parser encountered which caused the error.
  int unexpected_token() const;

  int token_position() const;

  int character_position() const;

  int line_position() const;

  // Returns an iterator to the beginning of a list of expected tokens.
  std::vector<int>::const_iterator expected_tokens_begin() const;

  // Returns an iterator to the end of a list of expected tokens.
  std::vector<int>::const_iterator expected_tokens_end() const;

  std::string unexpected_lexeme() const;
 private:
  int unexpectedToken, tokenPosition, characterPosition, linePosition;
  std::string unexpectedLexeme;
  std::vector<int> expectedTokens;
}
} // namespace parser
} // namespace dynsolver
#endif
