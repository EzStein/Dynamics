#ifndef DYNSOLVER_REGEX_LEXER_H_
#define DYNSOLVER_REGEX_LEXER_H_

#include <string>
#include <vector>

#include "regex/regex.h"
#include "regex/dfa.h"

namespace dynsolver {
namespace regex {

// The lexer class implements a basic tokenizer.
// The lexer contains an ordered list of rules which
// associate a regex pattern to an integer representing a token.
// The lexer attempts to read from the input string the longest prefix which
// is matched by a rule. If two patterns
// match the same longest lexeme then the first pattern in the list is chosen for
// the tokenization. Duplicate rules are allowed, but the second rule is
// effectively ignored. It is not possible to modify the rule set of the lexer
// after creation. However, it is possible to reset the input string and input
// pointer by using the set_input member function.
// In order to build a lexer, we use a lexer_builder object.
// Tokens are represented using integers.
// Note that the integers are meant to be converted to and from enum classes.
// These integers should be nonnegative since the value -1 is a special
// return value of the consume_token function. The behavior of this class
// is undefined if a negative value is given as a token (we assert an
// error in debug mode).
// 
// NOTE: the current implementation of the lexer is with a std::vector of
// rules. We simulate each rule independently and choose the token of the rule
// which has the longest matched prefix. A better implementation involves the creation
// of a special NFA with multiple accepting states which allow for parallel
// simulation of all the rules. However this is not yet supported in the NFA
// class.
class lexer {
 public:
  // Returns the longest lexeme by reference
  // and the associated token matched by one of the
  // rules. If two rules match the same longest lexeme, the one which was
  // added first is given preference. This function will advance the pointer
  // in order to consume the lexeme. If we have already consumed all the
  // characters in the input string (the input pointer is input.size()) then
  // the lexer still operates as though its input were the empty string. If
  // there are no matches, this function returns the value -1 and does not
  // change the lexeme reference and does not advance the input.
  int consume_token(std::string& lexeme);

  // Returns the longest lexeme and token as described above, but it does
  // not advance the input pointer. That is, multiple calls to this function
  // without a call to consume_token will return the same values.
  int peek_token(std::string& lexeme) const;

  // True if we have not yet consumed all the characters in the input string.
  // That is, the input pointer has not yet reached input.size().
  bool has_next() const;

  // Sets the input string and resets the input pointer to the string start.
  // The lexer class is a ruleset that operates on strings. Use this function
  // to process multiple inputs with the same ruleset. During construction,
  // the input string is the empty string and this method should be used
  // to set the initial input.
  void set_input(const std::string& input);

  // Returns the position of the first character in the lexeme
  // that would be returned by a call to consume_token.
  // The position is independent of the line number. That is, it does not
  // reset for each new line. Note that the first character has position 0.
  // If has_next returns false, we return 1 plus the position of the
  // last character in the last token.
  int character_position() const;

  // Currently this function returns 0.
  int line_position() const;

  // Returns the position of the token that would be returned by a call
  // to consume_token. The first token has position 0. If has_next returns
  // false, we return 1 plus the position of the last token.
  int token_position() const;
 private:
  // This struct represents one of the lexer's rules. It is essentially a
  // regex, int pair. It associates a regex pattern to its token.
  struct pattern_rule {
   public:
    pattern_rule(const dfa& pattern, int token)
        : patternObj(pattern), pattern(patternObj), token(token) { } 
    regex& pattern;
    int token;
   private:
    dfa patternObj;
  };

  // This struct is not actually used in the internal representation of the
  // lexer, but it is an intermediate, space efficient representation that
  // the lexer_builder constructs and then sends to the lexer constructor.
  // The constructor will convert each string_rule to a pattern_rule which
  // can simulate a regex.
  struct string_rule {
    std::string pattern;
    int token;
  };

  // This vector stores the ordered sequence of rules to apply to the stream
  // in order to find tokens.
  std::vector<pattern_rule> rules;

  // This string represents the lexer's input. It is set at construction and
  // cannot subsequently be changed.
  std::string input;
  
  // This integer points to the character in input which we are currently
  // examining. That is, a call to next_token will return a lexeme starting
  // with this character if possible.
  int pointer;

  // Used to record the number of tokens we have seen so far.
  int currentTokenPosition;
  
  // As an optimization, we only compute the next token once, regardless
  // of how many calls we make that don't consume input.
  // We thus need to store the nextToken and the nextPointer.
  int nextPointer;
  int nextToken;

  // We have a private method with sets the nextToken and nextPointer by
  // reading from the input. This function will not advance pointer.
  void set_next_token();

  // We currently only allow construction from the lexer_builder class which is
  // a friend of lexer. We construct lexer from a std::vector<rule> representing
  // the ordered list of rules to use.
  lexer(const std::vector<string_rule>& rules);
  
  friend class lexer_builder;
};
} // namespace regex
} // namespace dynsolver

#endif

