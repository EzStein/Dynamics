#ifndef SYMBOL_GUARD
#define SYMBOL_GUARD
#include <string>

/*
*  Symbols are entries in a symbol table.
*  They contain imformation for identifiers that are used as variables in expressions.
*  The id of the symbol is effectively the pointer to it in the list (symbol table).
*/

struct symbol {
  std::string name;

  /*True if the names are equal*/
  friend bool operator==(const symbol&, const symbol&);
};
#endif
