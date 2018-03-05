#ifndef DYNSOLVER_REGEX_REGEX_H_
#define DYNSOLVER_REGEX_REGEX_H_

#include <limits>
#include <string>

namespace dynsolver {
namespace regex {

// This interface class  may be used to
// simulate a regular expression to determine if a given string
// matches a given pattern. Additionally this class can find the longest prefix
// of a string that is matched by the regular expression. As such, greedy/lazy
// operators are not yet supported. This implementation supports the following
// regular expression specification based loosely on perl regex's.
//
// If a and b are patterns,
// a|b    matches a or b (alternation)
// ab     matches a concatenated with b
// a*     matches zero or more instances of a
//
// Parentheses are used to group matches by precedence.
// In addition to the preceding basic operators, the follow constructs
// are supported in this implementation:
// a?    matches zero or one instances of a
// a+    matches one or more instances of a
//
// Character classes are supported. If x, y, and z, are characters,
// [xyz]  matches x or y or z. Nested character classes are not supported.
// In fact, only literals may appear in a character class. Literals include
// the special character classes, any non-control character, and any
// control character and any escaped control character.
//
// Character classes with the [^xyz] syntax are not supported.
//
// The following are special character classes:
// \d    matches a digit character. That is, the characters 0123456789
// \D    matches a non-digit character.
// \s    matches a whitespace character. That is, a space, line feed, form feed
//       carriage return, horizontal tab, and vertical tab.             
// \S    matches a non-whitespace character.
// \w    matches an alphanumeric character. That is, an alphabet character,
//       or a digit character. Note that we EXCLUDE the underscore.
// \W    matches a non-alphanumeric character.
// .     matches any character.
//
// Note that the syntax [a-zA-Z] is not supported. Instead use
// \a    matches an alphabet character (uppercase or lowercase).
// \A    matches a non-alphabet character.
//
// Nested character classes are not supported however character classes
// may contain special character classes.
//
// The empty pattern matches only the empty string and nothing else.
//
// Note that in most regular expression engines there are escaped literals
// that represent newlines, tabs, control characters, etc...
// This implementation does not support such special literals. Note that
// this does not prevent you from embedding a special character, such as
// a new line character into a string using the c++ escape sequence "\n" or "\x0a"
//
// Note that all characters match themselves (including whitespace and non-printable
// characters) with the following exceptions:
// ()[]*+?|\.
//
// All of the above characters can be matched
// by preceding it with a backslash. Note that the backslash is also an escape
// character in c++. Thus in order to embed a backslash into a string in a c++
// literal, use "\\". In particular, to match a ( the following c++ literal
// should be used: "\\(". Somewhat frustratingly, to match a backslash
// we must provide the string "\\\\" in c++.
//
// In order to provide maximum flexibility, the null character may be embedded in the
// middle of a c++ string which can then be used as either a pattern or as a
// candidate match for the regular expression. If a c-style string is provided
// instead we will assume that any null character terminates the string,
// and it will not be included in either the pattern or the candidate match.
//
// Note that the this class and any class in the regex namespace are not aware
// of character encodings. These regex's merely operate on the bytes of the input
// patterns and candidate matches. In general, the character encoding used to write
// the pattern should be the same as the character encoding of the candidate matches.
// Be careful when reading from files in whose character encoding does not match the
// pattern's encoding. Throughout documentation, any character that is refered to
// will be represented internally using the ISO-8859 encoding scheme. Nearly,
// all character encodings will encode the first 128 characters of unicode
// the same way.
class regex {
 public:
  // Returns true if the provided string is matched by this regular expression
  bool accepts(const std::string& candidate) const;

  // Returns an integer indicating the length of the smallest
  // matched prefix of candidate if it exists.
  // We ignore characters whose indices are less than startPosition.
  // That is, this function has the effect of
  // finding the longest matched prefix of
  // candidate.substr(startPosition, candidate.size() - startPosition).
  // By default startPosition is 0 indicating that the whole string should
  // be considered. The startPosition value may be anywhere from 0 to
  // candidate.length() inclusive. If it is candidate.length(), then
  // we consider the empty string. If startPosition is out of range then
  // the behavior is undefined (we may assert an error in development mode).
  // 
  // The matched prefix if it exists can be recovered
  // using candidate.substr(startPosition, length) where
  // length is the returned integer. In particular the substring
  // spans from character startPosition (inclusive)
  // to character length (exclusive).
  // Note that if the empty string is the longest matched prefix,
  // this function returns 0. If the pattern does not match any prefix
  // (not even the empty string) then the value -1 is returned.
  virtual int accept_longest_prefix(const std::string& candidate,
                                    int startPosition = 0) const = 0;
};

} // namespace regex
} // namespace dynsolver
#endif
