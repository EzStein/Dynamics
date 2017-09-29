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
  std::stringstream ss("(x*3*y*2*x^2*(4+1)^0.3+2)");
  parser par(ss);
  list<symbol> l;
  AST ast = par.parse(l);
  ast.simplify();
  
  std::cout << ast << std::endl;
  return 0;
}
