#ifndef PARSER_COMMON_H_
#define PARSER_COMMON_H_
// Common files contain the symbols needed by interdependent headers.
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <limits>

namespace parser {
// Contains the number of terminal and nonterminal symbols.
// associated with a given production.
struct production_info {
  int terminalSymbols;
  int nonterminalSymbols;
};

const int kEndOfInputToken(std::numeric_limits<int>::min());
const int kGeneratedStartSymbol(std::numeric_limits<int>::min() + 1);
}
#endif
