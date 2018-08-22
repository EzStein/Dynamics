#include "regex/lexer_builder.h"

#include <string>
#include <vector>

#include "regex/lexer.h"

namespace regex {

lexer_builder::lexer_builder() { }

lexer_builder& lexer_builder::register_rule(const std::string& pattern,
					    int token) {
  rules.push_back(lexer::string_rule{pattern, token});
  return *this;
}

lexer lexer_builder::build() {
  return lexer(rules);
}
} // namespace regex
