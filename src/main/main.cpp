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

int mains() {
  while(1) {
    std::cout << "Derivative Calculator!"<<std::endl;
    std::cout << "Enter an expression: " << std::endl;
    std::string str;
    std::cin >> str;
    std::cout << "Enter a variable name (x, y, z, etc...): " << std::endl;
    std::string var;
    std::cin >> var;


    std::stringstream ss(str);
    parser par(ss);
    list<symbol> l;
    AST ast = par.parse(l);
    ast.differentiate(var);
    std::cout << "The derivative is:" << std::endl;
    std::cout << ast << std::endl;

  }

  return 0;
}
