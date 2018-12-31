#include "math/integer.h"

#include <iostream>
#include <cassert>
#include <sstream>

namespace dynsolver {
namespace math {

namespace {
// Converts the byte into its hex character. If the byte
// is larger than 15, the behavior is undefined.
char to_char(uint8_t byte) {
  switch(byte) {
    case 0x00: return '0';
    case 0x01: return '1';
    case 0x02: return '2';
    case 0x03: return '3';
    case 0x04: return '4';
    case 0x05: return '5';
    case 0x06: return '6';
    case 0x07: return '7';
    case 0x08: return '8';
    case 0x09: return '9';
    case 0x0A: return 'A';
    case 0x0B: return 'B';
    case 0x0C: return 'C';
    case 0x0D: return 'D';
    case 0x0E: return 'E';
    case 0x0F: return 'F';
    default: assert(false);
  }
  return '0';
}

// Converts a hex character into byte. If the character is
// not a digit or a capital letter A through F, then the behavior
// is undefined.
uint8_t from_char(char c) {
  switch(c) {
    case '0': return 0x00;
    case '1': return 0x01;
    case '2': return 0x02;
    case '3': return 0x03;
    case '4': return 0x04;
    case '5': return 0x05;
    case '6': return 0x06;
    case '7': return 0x07;
    case '8': return 0x08;
    case '9': return 0x09;
    case 'A': return 0x0A;
    case 'B': return 0x0B;
    case 'C': return 0x0C;
    case 'D': return 0x0D;
    case 'E': return 0x0E;
    case 'F': return 0x0F;
    default: assert(false);
  }
  return 0x00;
}
}

void integer::construct_integer(std::uintmax_t num) {
  data.clear();
  // We shift one more word than necessary to ensure that the result,
  // is positive.
  for(size_t i = 0; i != sizeof(num) / sizeof(word_t) + 1; ++i) {
    data.push_back(num & wordMax);

    // Set num to zero if wordBits == sizeof(num) * CHAR_BITS.
    // Otherwise num >>= wordBits would have undefined behavior.
    num >>= wordBits - 1;
    num >>= 1;
  }
  trim();
}

integer::integer(std::intmax_t num) {
  assert(num != INTMAX_MIN);
  construct_integer(static_cast<std::uintmax_t>(std::abs(num)));
  if(num < 0) twos_complement(); // Negates the integer.
}

bool integer::negative() const {
  // True if the most significant bit of the most significant
  // word is set.
  return data[data.size() - 1] & wordMsb;
}

void integer::trim() {
  if(negative()) {
    // Remove trailing wordMax words if the next most
    // significant byte has a set most significant bit.
    for(size_t i = data.size() - 1; i != 0; --i) {
      if(data[i] == wordMax && (data[i - 1] & wordMsb)) {
        data.pop_back();
      } else {
        break;
      }
    }
  } else {
    // Remove trailing 0 bytes if the next most
    // significant byte has an unset most significant bit.
    for(size_t i = data.size() - 1; i != 0; --i) {
      if(data[i] == 0 && !(data[i - 1] & wordMsb)) {
        data.pop_back();
      } else {
        break;
      }
    }
  }
}

std::string integer::hex_str() const {
  // Assume CHAR_BIT == 8. This is true on all modern platforms
  // which are not embedded devices.
  static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
  std::stringstream sstream;
  sstream << "0x";
  for(size_t i = data.size(); i-- != 0;) {
    for(size_t j = sizeof(word_t); j-- != 0;) {
      word_t mask = static_cast<word_t>(0xFF_w << (j * 8));
      uint8_t byte = static_cast<uint8_t>((data[i] & mask) >> (j * 8));
      sstream.put(to_char((byte & 0xF0u) >> 4));
      sstream.put(to_char(byte & 0x0Fu));
    }
  }
  return sstream.str();
}

integer& integer::operator-() {
  twos_complement();
  return *this;
}

void integer::twos_complement() {
  for(size_t i = 0; i != data.size(); ++i) {
    data[i] = ~data[i];
  }
  trim();
  operator+=(integer(static_cast<std::uintmax_t>(1)));
}

void integer::sign_extend(size_t newSize) {
  word_t extend = data[data.size() - 1] & wordMsb ? wordMax : 0_w;
  for(size_t i = data.size(); i < newSize; ++i) {
    data.push_back(extend);
  }
}

integer& integer::operator+=(const integer& other) {
  // We sign extend the data by at least one word larger than
  // either operand to ensure that overflow does not occur during
  // addition.
  size_t newSize = std::max(data.size(), other.data.size()) + 1;
  sign_extend(newSize);
  integer copy(other);
  copy.sign_extend(newSize);
  
  word_t carry = 0;
  for(size_t i = 0; i != newSize; ++i) {
    word_t tmp = data[i] + copy.data[i] + carry;
    // Compute the carry out. Carry out occurs if the
    // sum caused an overflow.
    if(carry) {
      carry = tmp <= data[i];
    } else {
      carry = tmp < data[i];
    }
    data[i] = tmp;
  }
  trim();
  return *this;
}

integer::word_t integer::high_mask(size_t n) {
  word_t acc = 0_w;
  for(size_t i = 0; i != n; ++i) {
    acc >>= 1;
    acc |= wordMsb;
  }
  return acc;
}

integer::word_t integer::low_mask(size_t n) {
  word_t acc = 0_w;
  for(size_t i = 0; i != n; ++i) {
    acc <<= 1;
    acc |= 1_w;
  }
  return acc;
}

void integer::shift_left(size_t exp) {
  assert(exp < wordBits);
  if(exp == 0) return;
  word_t outShiftMask = high_mask(exp);
  word_t inShift = 0_w;
  for(size_t i = 0; i != data.size(); ++i) {
    word_t outShift = (data[i] & outShiftMask) >> (wordBits - exp);
    data[i] <<= exp;
    data[i] |= inShift;
    inShift = outShift;
  }
  // Sign extend the inShift to the full word.
  if(inShift & (1_w << (exp - 1))) {
    inShift |= high_mask(wordBits - exp);
  }
  data.push_back(inShift);
}

integer& integer::operator<<=(size_t exp) {
  while(exp >= wordBits) {
    size_t shift = wordBits - 1;
    shift_left(shift);
    exp -= shift;
  }
  shift_left(exp);
  trim();
  return *this;
}

void integer::abs() {
  if(negative()) {
    twos_complement();
  }
  assert(!negative());
}

integer& integer::operator*=(const integer& ot) {
  integer other(ot);
  bool negativeProduct = (negative() != other.negative());
  abs();
  other.abs();
  integer acc(0);
  for(size_t i = 0; i != other.data.size(); ++i) {
    for(size_t j = 0; j != data.size(); ++j) {
      integer tmp(0);
      tmp.construct_integer(other.data[i] * data[j]);
      acc += tmp << (i * wordBits + j * wordBits);
    }
  }
  if(negativeProduct) {
    acc.twos_complement();
  }
  operator=(acc);
  trim();
  return *this;
}

integer operator*(integer a, const integer& b) {
  return a *= b;
}

integer& integer::operator-=(const integer& other) {
  return operator+=(-integer(other));
}

integer& integer::operator++() {
  return operator+=(integer(1));
}

integer& integer::operator--() {
  return operator-=(integer(1));
}

integer integer::operator++(int) {
  integer tmp(*this);
  operator++();
  return tmp;
}

integer integer::operator--(int) {
  integer tmp(*this);
  operator--();
  return tmp;
}

integer operator+(integer a, const integer& b) {
  return a += b;
}

integer operator-(integer a, const integer& b) {
  return a -= b;
}

bool operator==(const integer& a, const integer& b) {
  return a.data == b.data;
}

bool operator!=(const integer& a, const integer& b) {
  return !(a == b);
}

bool operator<(const integer& a, const integer& b) {
  return (a - b).negative();
}

bool operator<=(const integer& a, const integer& b) {
  return a < b || a == b;
}

bool operator>=(const integer& a, const integer& b) {
  return !(a < b);
}

bool operator>(const integer& a, const integer& b) {
  return a >= b && a != b;
}

integer::word_t operator""_w(unsigned long long a) {
  return static_cast<integer::word_t>(a);
}

std::ostream& operator<<(std::ostream& os, const integer& a) {  
  os << a.hex_str();
  return os;
}

integer operator<<(integer val, size_t n) {
  return val <<= n;
}
} // namespace math
} // namespace dynsolver
