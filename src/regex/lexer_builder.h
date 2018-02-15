#ifndef DYNSOLVER_REGEX_LEXER_BUILDER_H_
#define DYNSOLVER_REGEX_LEXER_BUILDER_H_

#include <string>
#include <vector>

#include "regex/lexer.h"

namespace dynsolver {
namespace regex {
// The lexer class is effectively immutable in the sense that the rules it uses
// for tokenization are set at construction and cannot be changed afterwords.
// In order to hide the implementation of the lexer's rules, we use a builder
// class which allows us to generate a lexer object which can not be mutated.
// The lexer_builder class allows us to register an arbitrary number of rules.
// The rules are string, integer pairs. The string is a regular expression
// and the integer is the rule identifier (one may think of this as the token
// associated to the regular expression). When all the desired rules are
// registered, a lexer object may be created. Note that the order in which rules
// are registered is important. If the lexer can match more than one rule/token
// to a lexeme, then the first rule registered is given precedence. The
// associated id (token) is returned.
class lexer_builder {
 public:
  // Default constructor
  lexer_builder();
  
  // Appends the provided rule to the end of the list of rules. Duplicate rules
  // are allowed even if they point to different tokens. In this case only the
  // first of the repeated rule will be used. This function returns a reference
  // to itself so that these function calls may be chained.
  // It is an error to use a negative value as the token paramter.
  lexer_builder& register_rule(const std::string& pattern, int token);

  // Constructs the lexer using the rule set and returns the result.
  // Note that the input to the lexer is by default set to the empty string.
  // The client should call lexer.set_input() to set the appropriate
  // input string before proceeding to use the lexer.
  lexer build();

 private:
  // Internally, the lexer builder generates a vector of string rules which it
  // then passes to the lexer constructor.
  std::vector<lexer::string_rule> rules;
};

} // namespace regex
} // namespace dynsolver
#endif
