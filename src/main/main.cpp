#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <map>
#include <cassert>
#include <sys/mman.h>
#include <string>
#include "math/vector.h"

#include "compiler/ast/AST.h"
#include "compiler/parser.h"
#include "compiler/front/driver.h"
#include "compiler/asm/assembler.h"
using namespace std;

int mains() {

  driver d;
  driver::var4_double_func_t func = d.compile_as_function<driver::var4_double_func_t>(
    std::string("x"));
  std::cout << func(3,-4,0,0) << std::endl;
  assembler assem;
  std::vector<unsigned char> buf;
  try {
    buf = assem.assemble("fyl2x");
  } catch(char * err) {
    std::cout << err << std::endl;
  } catch(const char * err) {
    std::cout << err << std::endl;
  } catch(std::string err) {
    std::cout << err << std::endl;
  }
  ofstream file;

  file.open("asm.S", ios::binary | ios::out);
  file.write(reinterpret_cast<char*>(buf.data()), buf.size());
  file.close();

  return 0;
}
