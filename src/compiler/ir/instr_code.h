#ifndef INSTR_CODE_GUARD
#define INSTR_CODE_GUARD
#include <list>
/*
* This is a class representing a three address code for intermediate code representation.
* The class is stored as triples. IE, one field is the operator,
* and the other two are the operator's arguments which are either
* pointers to another instr_code or a pointer to a symbol table.
*/
struct instr_code {
  typedef list<instr_code>::const_iterator instr_code_ptr_type;
  op op;
  union {
    instr_code_ptr_type operand1;
     operand1;
  }

  instr_code_ptr_type operand2;
};

#endif
