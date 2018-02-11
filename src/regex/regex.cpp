#include "regex/regex.h"

#include <string>

#include "regex/dfa.h"

namespace dynsolver {
namespace regex {
regex::regex(const std::string& pattern) : automaton(pattern) { }
bool regex::accepts(const std::string& candidate) const {
  return automaton.accepts(candidate);
}
int regex::accept_longest_prefix(const std::string& candidate,
                                 int startPosition) const {
  return automaton.accept_longest_prefix(candidate, startPosition);
}
} // namespace dynsolver
} // namespace regex
