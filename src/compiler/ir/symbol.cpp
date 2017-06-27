#include "compiler/symbol.h"

bool operator==(const symbol& sym1, const symbol& sym2) {
  return sym1.name == sym2.name;
}
