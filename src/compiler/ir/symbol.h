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

  /*Its parameter position for use in functions.
  Generally 0 indicates it is the first parameter of a function, 1 the second, etc..*/
  unsigned int parameter;

  /*True if the names are equal*/
  friend bool operator==(const symbol&, const symbol&);
};
#endif
