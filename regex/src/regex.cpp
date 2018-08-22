#include "regex/regex.h"

namespace regex {

bool regex::accepts(const std::string& candidate) const {
  return accept_longest_prefix(candidate) == candidate.size();
}
}
