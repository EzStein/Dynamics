#include <catch2/catch.hpp>

#include <iostream>
#include <climits>

#include "math/integer.h"

namespace dynsolver {
namespace math {

// For standard non corner case tests marked as only positive,
// or only negative, these variables 
constexpr std::intmax_t range = 5;

constexpr std::intmax_t lowNeg = INTMAX_MIN + 1;
constexpr std::intmax_t highNeg = lowNeg + range;

constexpr std::intmax_t lowPos = INTMAX_MAX - range;
constexpr std::intmax_t highPos = lowPos + range;

// Tests that simple construction and equality checks work.
TEST_CASE("integer: operator==",
          "[integer][operator==]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      if(i == j) {
        REQUIRE(integer(i) == integer(j));
      } else {
        REQUIRE(!(integer(i) == integer(j)));
      }
    }
  }
}

TEST_CASE("integer: operator!=",
          "[integer][operator!=]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      if(i != j) {
        REQUIRE(integer(i) != integer(j));
      } else {
        REQUIRE(!(integer(i) != integer(j)));
      }
    }
  }
}

TEST_CASE("integer: operator<",
          "[integer][operator<]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      if(i < j) {
        REQUIRE(integer(i) < integer(j));
      } else {
        REQUIRE(!(integer(i) < integer(j)));
      }
    }
  }
}

TEST_CASE("integer: operator<=",
          "[integer][operator<=]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      if(i <= j) {
        REQUIRE(integer(i) <= integer(j));
      } else {
        REQUIRE(!(integer(i) <= integer(j)));
      }
    }
  }
}

TEST_CASE("integer: operator>",
          "[integer][operator>]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      if(i > j) {
        REQUIRE(integer(i) > integer(j));
      } else {
        REQUIRE(!(integer(i) > integer(j)));
      }
    }
  }
}

TEST_CASE("integer: operator>=",
          "[integer][operator>=]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      if(i >= j) {
        REQUIRE(integer(i) >= integer(j));
      } else {
        REQUIRE(!(integer(i) >= integer(j)));
      }
    }
  }
}

TEST_CASE("Integer: operator+",
          "[integer][operator+]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowPos; j != highPos; ++j) {
      REQUIRE(integer(i) + integer(j) == integer(i+j));
    }
  }
}

TEST_CASE("Integer: operator-",
          "[integer][operator-]") {
  for(std::intmax_t i = lowNeg; i != highNeg; ++i) {
    for(std::intmax_t j = lowNeg; j != highNeg; ++j) {
      REQUIRE(integer(i) - integer(j) == integer(i-j));
    }
  }
}

TEST_CASE("integer: operator++ postfix", "[integer][operator++]") {
  int i = -10;
  integer acc(i);
  while(i != 10) {
    REQUIRE(acc++ == integer(i++));
    REQUIRE(acc == integer(i));
  }
}

TEST_CASE("integer: operator++ prefix", "[integer][operator++]") {
  int i = -10;
  integer acc(i);
  while(i != 10) {
    REQUIRE(++acc == integer(++i));
  }
}

TEST_CASE("integer: operator-- postfix", "[integer][operator--]") {
  int i = 10;
  integer acc(i);
  while(i != -10) {
    REQUIRE(acc-- == integer(i--));
    REQUIRE(acc == integer(i));
  }
}

TEST_CASE("integer: operator-- prefix", "[integer][operator--]") {
  int i = 10;
  integer acc(i);
  while(i != -10) {
    REQUIRE(--acc == integer(--i));
  }
}

TEST_CASE("integer: negative", "[integer][negative]") {
  for(int i = -10; i != 10; ++i) {
    REQUIRE(integer(i).negative() == i < 0);
  }
}

TEST_CASE("integer: operator- unary", "[integer][operator- unary]") {
  for(int i = -10; i != 10; ++i) {
    REQUIRE(integer(i) == -integer(-i));
  }
}

TEST_CASE("integer: operator<<: positive", "[integer][operator<<]") {
  for(size_t i = 0; i != 10; ++i) {
    REQUIRE(integer(i) << 1 == integer(i * 2));
  }
}

TEST_CASE("integer: operator<<: negative", "[integer][operator<<]") {
  for(int i = -10; i != 0; ++i) {
    REQUIRE(integer(i) << 1 == integer(i * 2));
  }
}

TEST_CASE("integer: operator<<: zero test", "[integer][operator<<]") {
  for(int i = -5; i != 5; ++i) {
    REQUIRE(integer(i) << 0 == integer(i));
  }
}

TEST_CASE("integer: operator<<: large power", "[integer][operator<<]") {
  for(size_t i = 0; i != 10; ++i) {
    for(int n = 0; n != sizeof(int) * CHAR_BIT - 1; ++n) {
      REQUIRE(integer(i) << n == integer(i << n));
    }
  }
}

TEST_CASE("integer: operator*: positive", "[integer][operator*]") {
  for(size_t i = 0; i != 10; ++i) {
    for(size_t j = 0; j != 10; ++j) {
      REQUIRE(integer(i) * integer(j) == integer(i * j));
    }
  }
}

TEST_CASE("integer: operator*: negative", "[integer][operator*]") {
  for(int i = -10; i != 0; ++i) {
    for(int j = -10; j != 0; ++j) {
      REQUIRE(integer(i) * integer(j) == integer(i * j));
    }
  }
}

TEST_CASE("integer: operator*: mixed", "[integer][operator*]") {
  for(int i = -5; i != 5; ++i) {
    for(int j = -5; j != 5; ++j) {
      REQUIRE(integer(i) * integer(j) == integer(i * j));
    }
  }
}

TEST_CASE("integer: abs", "[integer][abs]") {
  for(int i = -10; i != 10; ++i) {
    integer tmp(i);
    tmp.abs();
    REQUIRE(tmp == integer(std::abs(i)));
  }
}
} // namespace math
} // namespace dynsolver
