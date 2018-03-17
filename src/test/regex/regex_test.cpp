#include "test/catch.hpp"

#include <cctype>
#include <iostream>
#include <memory>

#include "regex/dfa.h"
#include "regex/nfa.h"
#include "regex/malformed_pattern_exception.h"
#include "regex/regex.h"

namespace dynsolver {
namespace test {
namespace regex {

using dynsolver::regex::regex;
using dynsolver::regex::nfa;
using dynsolver::regex::dfa;
using dynsolver::regex::kMaxCharacterValue;
using dynsolver::regex::malformed_pattern_exception;

enum class regex_type {
  NFA, DFA
};

std::unique_ptr<regex> get_regex_obj(const std::string& pattern, regex_type type) {
  switch(type) {
    case regex_type::NFA:
      return std::unique_ptr<regex>(new nfa(pattern));
    case regex_type::DFA:
      return std::unique_ptr<regex>(new dfa(pattern));
  }
}

void test_single_character(regex_type type) {
  std::unique_ptr<regex> expr;
  
  // Tests the most basic input: a single character pattern matches itself.
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    try {
      expr = get_regex_obj(pattern, type);
      REQUIRE(expr->accepts(pattern));
    } catch (const dynsolver::regex::malformed_pattern_exception& mpa) {
      bool control(pattern == "(" || pattern == ")"
                   || pattern == "[" || pattern == "]" || pattern == "*"
                   || pattern == "+" || pattern == "?" || pattern == "|"
                   || pattern == "\\");
      REQUIRE(control);
    }
  }
  
  // We also test that the empty string accepts itself
  expr = get_regex_obj("", type);
  REQUIRE(expr->accepts(""));
}

void test_escaped(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("\\(", type);
  REQUIRE(expr->accepts("("));
  expr = get_regex_obj("\\)", type);
  REQUIRE(expr->accepts(")"));
  expr = get_regex_obj("\\[", type);
  REQUIRE(expr->accepts("["));
  expr = get_regex_obj("\\]", type);
  REQUIRE(expr->accepts("]"));
  expr = get_regex_obj("\\+", type);
  REQUIRE(expr->accepts("+"));
  expr = get_regex_obj("\\*", type);
  REQUIRE(expr->accepts("*"));
  expr = get_regex_obj("\\?", type);
  REQUIRE(expr->accepts("?"));
  expr = get_regex_obj("\\|", type);
  REQUIRE(expr->accepts("|"));
  expr = get_regex_obj("\\\\", type);
  REQUIRE(expr->accepts("\\"));
  expr = get_regex_obj("\\.", type);
  REQUIRE(expr->accepts("."));
}

void test_special_character_class(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("\\d", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(std::isdigit(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj("\\D", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(!std::isdigit(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj("\\s", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(std::isspace(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj("\\S", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(!std::isspace(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj("\\a", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(std::isalpha(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj("\\A", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(!std::isalpha(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj(".", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    REQUIRE(expr->accepts(pattern));
  }

  expr = get_regex_obj("\\w", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(std::isalnum(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }

  expr = get_regex_obj("\\W", type);
  for(int i = 0; i <= kMaxCharacterValue; ++i) {
    std::string pattern(1, static_cast<unsigned char>(i));
    if(!std::isalnum(i))
      REQUIRE(expr->accepts(pattern));
    else
      REQUIRE(!expr->accepts(pattern));
  }
}

void test_alternation(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("a|b", type);
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("b"));
  REQUIRE(!expr->accepts("aa"));
  REQUIRE(!expr->accepts("bb"));
  REQUIRE(!expr->accepts("ab"));

  expr = get_regex_obj("a|b|c", type);
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("b"));
  REQUIRE(expr->accepts("c"));

  expr = get_regex_obj("a|bc|d", type);
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("bc"));
  REQUIRE(expr->accepts("d"));
  REQUIRE(!expr->accepts("b"));
  REQUIRE(!expr->accepts("c"));
}

void test_concatenation(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("ab", type);
  REQUIRE(!expr->accepts("a"));
  REQUIRE(!expr->accepts("b"));
  REQUIRE(!expr->accepts("aa"));
  REQUIRE(!expr->accepts("bb"));
  REQUIRE(expr->accepts("ab"));

  expr = get_regex_obj("aa", type);
  REQUIRE(!expr->accepts("a"));
  REQUIRE(!expr->accepts("b"));
  REQUIRE(!expr->accepts("bb"));
  REQUIRE(expr->accepts("aa"));

  expr = get_regex_obj("aba", type);
  REQUIRE(!expr->accepts("a"));
  REQUIRE(!expr->accepts("b"));
  REQUIRE(!expr->accepts("aa"));
  REQUIRE(!expr->accepts("bb"));
  REQUIRE(expr->accepts("aba"));
}

void test_kleene(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("a*", type);
  REQUIRE(expr->accepts(""));
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("aa"));
  REQUIRE(expr->accepts("aaa"));

  expr = get_regex_obj("a+", type);
  REQUIRE(!expr->accepts(""));
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("aa"));
  REQUIRE(expr->accepts("aaa"));

  expr = get_regex_obj("a?", type);
  REQUIRE(expr->accepts(""));
  REQUIRE(expr->accepts("a"));
  REQUIRE(!expr->accepts("aa"));
  REQUIRE(!expr->accepts("aaa"));
}

void test_parentheses(regex_type type) {
  std::unique_ptr<regex> expr;
  try {
    expr = get_regex_obj("()", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  
  expr = get_regex_obj("(a)", type);
  REQUIRE(expr->accepts("a"));

  expr = get_regex_obj("a(b|c)d", type);
  REQUIRE(expr->accepts("abd"));
  REQUIRE(expr->accepts("acd"));
  REQUIRE(!expr->accepts("aad"));

  expr = get_regex_obj("a[bc]d", type);
  REQUIRE(expr->accepts("abd"));
  REQUIRE(expr->accepts("acd"));
  REQUIRE(!expr->accepts("aad"));

  expr = get_regex_obj("(ad)?", type);
  REQUIRE(expr->accepts(""));
  REQUIRE(expr->accepts("ad"));
  REQUIRE(!expr->accepts("adad"));

  expr = get_regex_obj("(ad)*", type);
  REQUIRE(expr->accepts(""));
  REQUIRE(expr->accepts("ad"));
  REQUIRE(expr->accepts("adad"));

  expr = get_regex_obj("(a|d)*", type);
  REQUIRE(expr->accepts(""));
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("d"));
  REQUIRE(expr->accepts("ad"));
  REQUIRE(expr->accepts("aa"));
  REQUIRE(expr->accepts("da"));
  REQUIRE(expr->accepts("dd"));
}

void test_character_class(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("[abcd]", type);
  REQUIRE(expr->accepts("a"));
  REQUIRE(expr->accepts("b"));
  REQUIRE(expr->accepts("c"));
  REQUIRE(expr->accepts("d"));
  REQUIRE(!expr->accepts("ab"));
  REQUIRE(!expr->accepts("abcd"));

  expr = get_regex_obj("[\\)\\(\\]\\[]", type);
  REQUIRE(expr->accepts(")"));
  REQUIRE(expr->accepts("("));
  REQUIRE(expr->accepts("]"));
  REQUIRE(expr->accepts("["));
  REQUIRE(!expr->accepts("\\"));
  REQUIRE(!expr->accepts("."));

  try {
    expr = get_regex_obj("[]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[+]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[)]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[(]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[+]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[*]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[?]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  try {
    expr = get_regex_obj("[[]]", type);
    REQUIRE(false);
  } catch(const malformed_pattern_exception& mpe) { }
  
}

void test_prefix(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("hello", type);
  REQUIRE(expr->accept_longest_prefix("") == -1);
  REQUIRE(expr->accept_longest_prefix("hell") == -1);
  REQUIRE(expr->accept_longest_prefix("hello") == 5);
  REQUIRE(expr->accept_longest_prefix("hello world!") == 5);

  expr = get_regex_obj("(hello)*", type);
  REQUIRE(expr->accept_longest_prefix("") == 0);
  REQUIRE(expr->accept_longest_prefix("hell") == 0);
  REQUIRE(expr->accept_longest_prefix("hello") == 5);
  REQUIRE(expr->accept_longest_prefix("hello world!") == 5);
  REQUIRE(expr->accept_longest_prefix("hellohello") == 10);
  REQUIRE(expr->accept_longest_prefix("hellohello world!") == 10);

  expr = get_regex_obj("(hello)+", type);
  REQUIRE(expr->accept_longest_prefix("") == -1);
  REQUIRE(expr->accept_longest_prefix("hell") == -1);
  REQUIRE(expr->accept_longest_prefix("hello") == 5);
  REQUIRE(expr->accept_longest_prefix("hello world!") == 5);
  REQUIRE(expr->accept_longest_prefix("hellohello") == 10);
  REQUIRE(expr->accept_longest_prefix("hellohello world!") == 10);

  expr = get_regex_obj("-?(\\d)+(.(\\d)+)?", type);
  REQUIRE(expr->accept_longest_prefix("") == -1);
  REQUIRE(expr->accept_longest_prefix("-") == -1);
  REQUIRE(expr->accept_longest_prefix("-1") == 2);
  REQUIRE(expr->accept_longest_prefix("123") == 3);
  REQUIRE(expr->accept_longest_prefix("hello123") == -1);
  REQUIRE(expr->accept_longest_prefix("-234.666") == 8);
}

void test_regression(regex_type type) {
  std::unique_ptr<regex> expr;
  expr = get_regex_obj("^", type);
  REQUIRE(expr->accepts("^"));
}

TEST_CASE("REGEX NFA: Prefix") {
  test_prefix(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Single Character") {
  test_single_character(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Special Character Class") {
  test_special_character_class(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Character Class") {
  test_character_class(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Alternation") {
  test_alternation(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Concatenation") {
  test_concatenation(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Kleene") {
  test_kleene(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Escaped") {
  test_escaped(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Parentheses") {
  test_parentheses(regex_type::NFA);
}

TEST_CASE("REGEX NFA: Regression") {
  test_prefix(regex_type::NFA);
}

TEST_CASE("REGEX DFA: Single Character") {
  test_single_character(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Special Character Class") {
  test_special_character_class(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Character Class") {
  test_character_class(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Alternation") {
  test_alternation(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Concatenation") {
  test_concatenation(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Kleene") {
  test_kleene(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Escaped") {
  test_escaped(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Parentheses") {
  test_parentheses(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Prefix") {
  test_prefix(regex_type::DFA);
}

TEST_CASE("REGEX DFA: Regression") {
  test_prefix(regex_type::DFA);
}
} // namespace regex
} // namespace test
} // namespace dynsolver
