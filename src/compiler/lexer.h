#ifndef LEXER_GUARD
#define LEXER_GUARD
#include <iostream>
#include <map>
#include "reg_def.h"
#include "compiler/token.h"
#include "compiler/NFA.h"

class lexer {
public:
  lexer();

  /*Constructs a lexer with the given input stream of characters
  * and a rule set for identifying tokens*/
  lexer(std::istream*, const std::map<std::string, token>&);

  /*Attempts to read the longest matchable lexeme in the stream. If it exists, it, returns
  that lexeme along with its corresponding token. If no such lexeme could be found but there is still input left in the stream,
  the string containing the null character is returned along with the token ERROR. If there is no input left in the stream,
  the token ENDPOINT is returned along with the string containing the null character*/
  token next_token(std::string& lexeme);

  /*Returns the next token as described above without advancing the stream*/
  token peek(std::string& lexeme);

  /*Returns the previous lexeme and token, ie, those that were returned by the last call to next_token().
  If next_token() has not yet been called, this function returns the ERROR token and sets the lexeme to the empty string.
  This function does not alter the stream*/
  token previous(std::string& lexeme);
private:
  std::istream* stream;
  NFA fa;
  token previousToken;
  std::string previousLexeme;
  token nextToken;
  std::string nextLexeme;
  /*Maps accepting states to tokens*/
  std::map<state_type, token> tokenMap;
};
#endif
