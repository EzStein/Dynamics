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
#include "compiler/ast/visitor.h"
using namespace std;

class test_visitor : public visitor {
public:
};

int main() {
  /*std::stringstream ss("1-2+-2");
  parser par(ss);
  list<symbol> l;
  AST ast = par.parse(l);
  ast.simplify();
  std::cout << ast << std::endl;
  //driver::var4_double_func_t func =  dr.compile_as_function<driver::var4_double_func_t>("2.5^3.5");
  //std::cout << func(1,2,3,4) << std::endl;
  return 0;*/
  test_visitor vist;
 // integer_number_leaf_node n(0);
 // vist.visit(&n);
  return 0;
}
