#include "regex/lexer.h"

#include <cassert>
#include <string>
#include <vector>

namespace dynsolver {
namespace regex {
lexer::lexer(const std::vector<string_rule>& stringRules)
    : input(""), pointer(0), currentTokenPosition(0) {
  // We initilialize rules by converting the each string in stringRules
  // to a regex pattern. The rules vector is already reserved to the
  // appropriate size.
  for(std::vector<string_rule>::const_iterator iter = stringRules.begin();
      iter != stringRules.end(); ++iter) {
    assert(iter->token >= 0);
    rules.push_back(pattern_rule{regex(iter->pattern), iter->token});
  }
  
  // We initially set the pointer to 0. Then we load in the first token by
  // calling set_next_token();
  set_next_token();
}

bool lexer::has_next() const {
  return pointer != input.size();
}

void lexer::set_next_token() {
  // We iterate through all the rules and keep track of the longest prefix
  // and the associated token.
  int longestPrefix = -1;
  int token = -1;
  for(std::vector<pattern_rule>::const_iterator iter = rules.begin();
      iter != rules.end(); ++iter) {
    int tmp = iter->pattern.accept_longest_prefix(input, pointer);

    // We only update our token and longestPrefix if the new one is strictly
    // larger than the old. If they are the same, we defer to the previously
    // held value. If there is no match, then clearly the if body does not
    // execute. As a result, if no rules matchm token remains -1.
    if(tmp > longestPrefix) {
      longestPrefix = tmp;
      token = iter->token;
    }
  }
  if(longestPrefix != -1) {
    nextPointer = pointer + longestPrefix;
  }  
  nextToken = token;
}

int lexer::character_position() const {
  return pointer;
}

int lexer::line_position() const {
  return 0;
}

int lexer::token_position() const {
  return currentTokenPosition;
}

int lexer::consume_token(std::string& lexeme) {
  ++currentTokenPosition;
  
  // We return the nextToken and associated lexeme. We then advance the pointer
  // and read in another token.
  lexeme = std::string(input, pointer, nextPointer - pointer);
  int token = nextToken;

  // If the last call to set_next_token did not offer a match or matched only
  // the empty string, then all subsequent calls to set_next_token will do
  // the same. In particular, pointer == nextPointer.
  if(pointer != nextPointer) {
    pointer = nextPointer;
    set_next_token();
  }
  return token;
}

int lexer::peek_token(std::string& lexeme) const {
  lexeme = std::string(input, pointer, nextPointer - pointer);
  return nextToken;
}

void lexer::set_input(const std::string& input_) {
  // Reset the input. We also make the pointer point to the start of the new
  // input string.
  input = input_;
  pointer = 0;
}

} // namespace regex
} // namespace dynsolover
