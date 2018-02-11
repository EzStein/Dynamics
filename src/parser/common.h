#ifndef DYNSOLVER_PARSER_COMMON_H_
#define DYNSOLVER_PARSER_COMMON_H_
// Common files contain the symbols needed by interdependent headers.

// Contains the number of terminal and nonterminal symbols.
// associated with a given production.
struct production_info {
  int terminalSymbols;
  int nonterminalSymbols;
};
#endif
