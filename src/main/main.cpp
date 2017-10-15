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

int main() {

  assembler assem;
  std::vector<unsigned char> buf;
  std::string code =
  "addq $0x122, 8(%r8d)\n"
  "fldpi";
  try {
    buf = assem.assemble(code);
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
