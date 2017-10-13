#ifndef LEXER_GUARD
#define LEXER_GUARD
#include <iostream>
#include <map>
#include "compiler/regex/NFA.h"
#include "compiler/regex/reg_def.h"
#include "compiler/lex/token.h"

class lexer {
public:

  /*Constructs a lexer with the given input stream of characters
  * and a rule set for identifying tokens.
   If format operators is set to true, the lexer will  insert asterisks and minus signs
   in appropriate places, otherwise it performs no preformatting*/
  lexer(std::istream*, const std::map<std::string, token>&, bool formatOperators = true);

  /*Attempts to read the longest matchable lexeme in the stream. If it exists, it, returns
  that lexeme along with its corresponding token. If no such lexeme could be found but there is still input left in the stream,
  the string containing the null character is returned along with the token ERROR. If there is no input left in the stream,
  the token ENDPOINT is returned along with the string containing the null character

  * If formatOperators is true, the lexer is responsible for inserting asterisks when none are provided inbetween groupings of parenthesis.
  * In general, if a LEFT_PAREN is preceded by a token that is not an operator the stream start, an asterisk is inserted.
  * If a RIGHT_PAREN is followed by a token that is not an operator or endpoint, an asterisk is inserted.
  * The lexer also distinguishes between binary and unary minus. In general, if a minus sign is PRECEDED by
  * the start of the stream an operator or a left parenthesis it is a unary minus. If it is preceded by a NUMBER,
  * ID, or RIGHT_PAREN, it is a binary minus.
  */
  token next_token(std::string& lexeme);

  /*Returns the next token as described above without advancing the stream*/
  token peek(std::string& lexeme);

  /*Returns the previous lexeme and token, ie, those that were returned by the last call to next_token().
  If next_token() has not yet been called, this function returns the ERROR token and sets the lexeme to the empty string.
  This function does not alter the stream*/
  token previous(std::string& lexeme);
private:
  bool formatOperators;
  std::istream* stream;
  NFA fa;
  token previousToken;
  std::string previousLexeme;
  token nextToken;
  std::string nextLexeme;
  /*Maps accepting states to tokens*/
  std::map<state_type, token> tokenMap;

  /*Maintains the position of the stream before a call to set_next_token*/
  std::ios::pos_type currPos;

  void set_next_token();
};
#endif
