#include "driver.h"
#include "compiler/ast/AST.h"
#include "compiler/parser.h"
#include <string>
#include <sstream>
#include <iostream>

using std::stringstream;
using std::string;

driver::float_function driver::compile_as_float_function(string str) {
  stringstream sstream(str);
  parser parse(sstream);
  AST ast(parse.parse());
  unsigned int size = ast.code_size();
  unsigned char * buf = reinterpret_cast<unsigned char *>(mmap(NULL, size,
    PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
  ast.emit_code(std::cout, buf);
  double (*func)(double) = 0;
  *reinterpret_cast<void**>(&func) = buf;
  return func;
}

template<class FUNC>
void driver::delete_function(FUNC func, size_t size) {
  munmap(reinterpret_cast<void*>(func), size);
}

template void driver::delete_function<driver::float_function>(driver::float_function, size_t);
