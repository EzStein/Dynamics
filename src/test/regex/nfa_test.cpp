#include "test/catch.hpp"
#include "regex/nfa.h"

TEST_CASE("My First Test") {
  nfa regex("a");
  REQUIRE(regex.accept_longest_prefix("a") == 1);
}
