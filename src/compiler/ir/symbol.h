#ifndef SYMBOL_GUARD
#define SYMBOL_GUARD
#include <string>
#include <list>
/*
*  Symbols are entries in a symbol table.
*  They contain imformation for identifiers that are used as variables in expressions.
*  The id of the symbol is effectively the pointer to it in the list (symbol table).
*/

struct symbol {
  symbol(std::string _name) : name(_name) { };
  typedef std::list<symbol>::const_iterator ptr_type;
  std::string name;
  int id;
  /*True if the names are equal*/
  friend bool operator==(const symbol&, const symbol&);
};
#endif
