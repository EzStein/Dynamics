#ifndef DYNSOLVER_MATH_INTEGER_H_
#define DYNSOLVER_MATH_INTEGER_H_
#include <climits>
#include <limits>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

namespace dynsolver {
namespace math {

// This class represents an arbitrary precision integer.
// The integer is stored as an array of bytes in two's complement
class integer {
 private:
  // word_t is the largest unsigned integral type that does not incur
  // a serious performance penalty.
  typedef std::uint_fast32_t word_t;

  // Number of bits in a word_t.
  static constexpr size_t wordBits = sizeof(word_t) * CHAR_BIT;

  // The maximum word_t value.
  static constexpr word_t wordMax = std::numeric_limits<word_t>::max();

  // The word_t with only the most significant bit set.
  static constexpr word_t wordMsb =
      static_cast<word_t>(1) << (wordBits - 1);

  friend integer::word_t operator""_w(unsigned long long);

  // The integer is stored as a contiguous array of logically sized
  // chucnks called words in two's complement form. The least significant,
  // word is stored first so we may think of this integer as being
  // in little endian form. However, no assumptions are made about
  // the data encoding of each word. For instance, the word type maybe
  // be larger than one byte and thus may have its own endianness.
  //
  // We use the minimal number of words that convey information.
  // That is, the most significant byte is never
  // std::numeric_limits<word_t>::max() or 0
  // (std::numeric_limits<word_t>::min()) unless such a word is needed
  // to convey sign information or the integer is itself the number 0.
  std::vector<word_t> data;

  // Trims the integer such that it remains in canonical form.
  // Removes trailing words that do not add any information.
  void trim();

 public:
  // Constructs this integer from the provided one.
  integer(std::uintmax_t);

  // Returns true if the integer is negative.
  bool negative() const;

  // Returns the hex string of this integer.
  std::string hex_str() const;
  
  // Adds this integer to the provided integer.
  void operator+=(const integer& other);
};

integer::word_t operator""_w(unsigned long long a);
} // namespace math
} // namespace dynsolver
#endif
