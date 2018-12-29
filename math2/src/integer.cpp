#include "math/integer.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <climits>
#include <limits>
#include <string>
#include <sstream>
#include <cassert>

namespace dynsolver {
namespace math {

namespace {
// Converts the byte into a hex character. If the byte
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

integer::integer(std::uintmax_t num) {
  // We shift one more word than necessary to ensure that the result,
  // is positive.
  for(size_t i = 0; i != sizeof(num) / sizeof(word_t) + 1; ++i) {
    data.push_back(num & wordMax);
    num >>= wordBits;
  }
  trim();
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
  // Assume CHAR_BIT == 8
  static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
  std::stringstream sstream;
  for(size_t i = data.size(); i-- != 0;) {
    for(size_t j = sizeof(word_t); j-- != 0;) {
      word_t mask = 0xFF_w << (j * 8);
      std::cout << std::hex << mask << std::endl;
      uint8_t byte = static_cast<uint8_t>((data[i] & mask) >> (j * 8));
      sstream.put(to_char((byte & 0xF0u) >> 4));
      sstream.put(to_char(byte & 0x0Fu));
    }
  }
  return sstream.str();
}

void integer::operator+=(const integer& other) {
  size_t maxSize = std::max(data.size(), other.data.size());
  word_t carry = 0;
  for(size_t i = 0; i != maxSize; ++i) {
    word_t tmp = data[i] + other.data[i] + carry;
    if(carry) {
      carry = tmp <= data[i];
    } else {
      carry = tmp < data[i];
    }
    data[i] = tmp;
  }
}

integer::word_t operator""_w(unsigned long long a) {
  return static_cast<integer::word_t>(a);
}
} // namespace math
} // namespace dynsolver
