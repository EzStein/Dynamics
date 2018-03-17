// This file is for a main executable that is used for ad hoc tests and GDB debugging.

#include "regex/nfa.h"
#include "compiler/front/driver.h"
#include "compiler/ir/symbol.h"
#include "compiler/ast/AST.h"

#include <unordered_map>
struct production_info {
  int a;
  int b;
};
int main() {
  driver dr;
  std::list<symbol> table;
  table.push_back(symbol("x",0,0));
  table.push_back(symbol("y",1,1));
  table.push_back(symbol("a",2,2));
  AST ast = dr.parse_as_ast("a(y-x)",table);
  driver::double_func_t f = dr.compile_as_function<driver::double_func_t>(ast);
  double args[2] = {3,2};
  std::cout << "RESULT: " <<  f(args) << std::endl;
  return 0;
}
