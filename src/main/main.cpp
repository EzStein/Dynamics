#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <map>
#include <cassert>
#include <sys/mman.h>
#include "math/vector.h"

#include "compiler/ast/AST.h"
#include "compiler/parser.h"
#include "compiler/front/driver.h"
using namespace std;

int main() {
  std::stringstream ss("1+2+3");
  parser par(ss);
  list<symbol> l;
  AST ast = par.parse(l);
  //ast.simplify();
  std::cout << ast << std::endl;
  //driver::var4_double_func_t func =  dr.compile_as_function<driver::var4_double_func_t>("2.5^3.5");
  //std::cout << func(1,2,3,4) << std::endl;
  return 0;
}
