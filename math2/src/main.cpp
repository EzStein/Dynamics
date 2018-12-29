#include <iostream>

#include "math/group.h"
#include "math/integer.h"

using namespace dynsolver::math;

class int_group : public group<int> {
  int zero() const override {
    return 0;
  }

  int plus(const int&a, const int& b) const override {
    return a + b;
  }

  int add_inv(const int& a) const override {
    return -a;
  }

  bool equal(const int& a, const int& b) const override {
    return a == b;
  }
};



template<class T>
T apply_bin_op(const T& a, const T& b, const magma<T>& op) {
  return op.plus(a, b);
}

int main(void) {
  integer myInt(0xFF0912345);
  std::cout << myInt.hex_str() << std::endl;
  return 0;
}
