///
/// \file integer.h
/// \author Ezra Stein
/// Definition of the \link integer \endlink class for arbitrary
/// precision integers.
///
#ifndef DYNSOLVER_MATH_INTEGER_H_
#define DYNSOLVER_MATH_INTEGER_H_
#include <climits>
#include <limits>
#include <string>
#include <vector>

namespace dynsolver {
namespace math {

/// The \c integer class represents an arbitrary precision integer.
/// That is, an integer of any size positive or negative. The only
/// limitation on the size of the integer is the memory constraints
/// of the system.
class integer {
 private:
  /// The largest unsigned integral type that does not incur
  /// a serious performance penalty. The integer value is stored
  /// as an array of this type.
  typedef unsigned int word_t;

  /// Number of bits in a word_t.
  ///
  static constexpr size_t wordBits = sizeof(word_t) * CHAR_BIT;

  /// The maximum word_t value. This will always have a bit pattern
  /// filled with all 1's. That is, it is of the form 0xFFFFFFF...
  static constexpr word_t wordMax = std::numeric_limits<word_t>::max();

  /// The word_t with only the most significant bit set.
  ///
  static constexpr word_t wordMsb =
      static_cast<word_t>(1) << (wordBits - 1);

  /// Generates the \link word_t \endlink whose highest \c n number of
  /// bits are set to 1 and all other bits set to zero.
  /// \param n The number of bits to set. If \c n is larger than
  /// the number of bits available in a \link word_t \endlink,
  /// all the bits are set.
  /// \return A \link word_t \endlink where the highest \p n bits are
  /// set and all others are unset.
  static word_t high_mask(size_t n);

  /// Generates the \link word_t \endlink whose lowest \c n number of
  /// bits are set to 1 and all other bits set to zero.
  /// \param n The number of bits to set. If \c n is larger than
  /// the number of bits available in a \link word_t \endlink,
  /// all the bits are set.
  /// \return A \link word_t \endlink where the lowest \p n bits are
  /// set and all others are unset.
  static word_t low_mask(size_t n);

  /// The integer is stored as a contiguous array of logically sized
  /// chunks called words in two's complement form. The least significant,
  /// word is stored first so we may think of this integer as being
  /// in little endian form. However, no assumptions are made about
  /// the data encoding of each word. For instance, the word type maybe
  /// be larger than one byte and thus may have its own endianness.
  /// \invariant After any public member function call, the data
  /// will be in canonical form. Canonical form is defined as the
  /// minimal number of words needed to convey information.
  /// That is, the most significant byte is never
  /// <tt>std::numeric_limits<word_t>::max()</tt> or 0
  /// (<tt>std::numeric_limits<word_t>::min()</tt>)
  /// unless such a word is needed
  /// to convey sign information or the integer is itself the number 0.
  std::vector<word_t> data;

  /// Trims the integer such that it remains in canonical form by
  /// removing trailing words from \link data \endlink
  /// that do not add any information.
  void trim();

  /// Performs a two's complement on the integer value. That is,
  /// all bits are complemented and then 1 is added to the integer.
  void twos_complement();

  /// The user defined literal has access to the \link word_t \endlink
  /// type.
  friend word_t operator""_w(unsigned long long);

  /// The equality operator will have access to the \link data \endlink
  /// field.
  friend bool operator==(const integer&, const integer&);

  /// Constructs this integer from an unsigned integral type.
  /// \param num The number which is used to construct this integer.
  void construct_integer(std::uintmax_t num);

  /// Sign extends the integer to the appropriate number of
  /// words. If \p newSize is smaller than the current size
  /// of the integer, nothing happens. Sign extension
  /// preserves the value of the integer.
  /// \param newSize The number of words the resulting
  /// integer will contain after the sign extend unless
  /// \p newSize is less than \p data.size().
  void sign_extend(size_t newSize);

  /// Shifts the integer bits left by \p n.
  /// \param n The number of bits to shift less by. Must be strictly
  /// less than \link wordBits \endlink.
  /// \warning If \p n is greater than or equal to \link wordBits \endlink
  /// the behavior is undefined.
  void shift_left(size_t n);

 public:
  /// Constructs this integer from a signed integral type.
  /// \param num The number which is used to construct this integer.
  /// \warning The behavior is undefined if
  /// <tt>num == INTMAX_MIN</tt>, the minimum allowable value
  /// for the \c intmax_t type.
  explicit integer(std::intmax_t num);
  

  /// Tests whether this integer is negative.
  /// \return \c true if the integer is negative,
  /// \c false otherwise.
  bool negative() const;

  /// Takes the absolute value of this integer.
  /// \note This function mutates the object that calls it.
  void abs();

  /// Returns the hex string of this integer. No guarentees are made
  /// as to the formatting of this string only that it is a two's
  /// complement representation of the integer.
  /// \return A string representing this integer in hexadecimal.
  std::string hex_str() const;

  /// Multiplies this integer by \p other storing the result
  /// in \c this.
  /// \p other The integer which which we multiply with this one.
  /// \return A reference to \p this, the recently modified integer.
  integer& operator*=(const integer& other);

  /// Multiplies this integer by \f$2^{exp}\f$. This may be thought
  /// as a bit shift left for positive integer values.
  /// \param exp The number of times this integer is multiplied
  /// by 2. That is, it is the exponent in \f$2^{exp}\f$.
  /// \return A reference to \c this, the newly modified integer.
  integer& operator<<=(size_t exp);

  /// Adds another integer to this one.
  /// \param other The integer which is added to this one.
  /// \return A reference to \p this, the newly modified integer.
  integer& operator+=(const integer& other);

  /// Subtracts the integer \p other from this integer returning,
  /// a reference to the result (\p this).
  /// \param other The integer which is subtracted from this one.
  /// \return A reference to \p this, the newly modified integer.
  integer& operator-=(const integer& other);

  /// Unary minus operator that negates the current value of the
  /// integer.
  /// \return A reference to the integer itself.
  integer& operator-();

  /// Prefix increment operator for incrementing this integer by one.
  /// \return A reference to \p this, the newly modified integer.
  integer& operator++();

  /// Prefix decrement operator for decrementing this integer by one.
  /// \return A reference to \p this, the newly modified integer.
  integer& operator--();

  /// Postfix increment operator for incrementing this integer by one.
  /// \return A copy of the integer before the increment.
  integer operator++(int);

  /// Postfix decrement operator for decrementing this integer by one.
  /// \return A copy of the integer before the decrement.
  integer operator--(int);
};

/// A user defined integer literal which converts any integer
/// to a word_t type. To use, append _w to the end of any
/// integer literal.
integer::word_t operator""_w(unsigned long long a);

/// Computes the sum of the integers \p a and \p b.
/// \param a The left operand of the sum.
/// \param b The right operand of the sum.
/// \return The sum of the integers \p a and \p b.
integer operator+(integer a, const integer& b);

/// Computes the product of the integers \p a and \p b.
/// \param a The left operand of the product.
/// \param b The right operand of the product.
/// \return The product of the integers \p a and \p b.
integer operator*(integer a, const integer& b);

/// Subtracts \p b from \p a and returns the result.
/// \param a The value from which \p b is subtracted.
/// \param b The value subtracted from \p a.
/// \return The value of \p b subtracted from \p a.
integer operator-(integer a, const integer& b);

/// Compares the two integers for equality.
/// \param a The left operand of the equality check.
/// \param b The right operand of the equality check.
/// \return \c true if the two integers have the same value,
/// \c false otherwise.
bool operator==(const integer& a, const integer& b);

/// Compares the two integers for inequality.
/// \param a The left operand of the inequality check.
/// \param b The right operand of the inequality check.
/// \return \c false if the two integers have the same value,
/// \c true otherwise.
bool operator!=(const integer& a, const integer& b);

/// Compares the two integers.
/// \param a The left operand of the comparison.
/// \param b The right operand of the comparison.
/// \return \c true if \p a is less than \p b. \c false otherwise.
bool operator<(const integer& a, const integer& b);

/// Compares the two integers.
/// \param a The left operand of the comparison.
/// \param b The right operand of the comparison.
/// \return \c true if \p a is less than or equal to \p b.
/// \c false otherwise.
bool operator<=(const integer& a, const integer& b);

/// Compares the two integers.
/// \param a The left operand of the comparison.
/// \param b The right operand of the comparison.
/// \return \c true if \p a is greater than \p b. \c false otherwise.
bool operator>(const integer& a, const integer& b);

/// Compares the two integers.
/// \param a The left operand of the comparison.
/// \param b The right operand of the comparison.
/// \return \c true if \p a is greater than or equal to \p b.
/// \c false otherwise.
bool operator>=(const integer& a, const integer& b);

/// Prints the hex string of \p a to the output stream.
/// \param os The output stream to which we print the integer.
/// \param a The integer to print to the stream.
/// \return A reference to the output stream, \p os.
std::ostream& operator<<(std::ostream& os, const integer& a);

/// Multiplies the integer \p val by \f$2^n\f$. This may be thought
/// as an arithmetic bit shift left.
/// \param val The integer to multiply by \f$ 2^n \f$
/// \param n The number of times this integer is multiplied
/// by 2. That is, it is the exponent in \f$2^n\f$.
/// \return The value \f$ val * 2^n \f$.
integer operator<<(integer val, size_t n);
} // namespace math
} // namespace dynsolver
#endif
