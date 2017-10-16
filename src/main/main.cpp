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
  std::string str("z+1*x+y");
  stringstream sstream(str);
  parser parse(sstream);

  list<symbol> symbolTable;
  /*Compile the function*/
  AST ast(parse.parse(symbolTable));
  ast.simplify();
  std::cout << ast << std::endl;
  assembler assem;
  std::vector<unsigned char> buf;
  str =
  "pushq %rbp\n"
  "movq %rsp, %rbp\n"
  "subq $0x50, %rsp\n"
  "stmxcsr -0x4(%rbp)\n"
  "fstcw -0x6(%rbp)\n"
  "movw $0x0f7f, -0x8(%rbp)\n"
  "movq %xmm0, -0x48(%rbp)\n"
  "movq %xmm1, -0x40(%rbp)\n"
  "movq %xmm2, -0x38(%rbp)\n"
  "movq %xmm3, -0x30(%rbp)\n"
  "movq %xmm4, -0x28(%rbp)\n"
  "movq %xmm5, -0x20(%rbp)\n"
  "movq %xmm6, -0x18(%rbp)\n"
  "movq %xmm7, -0x10(%rbp)\n"
  "pushq %rbx\n"
  "pushq %r12\n"
  "pushq %r13\n"
  "pushq %r14\n"
  "pushq %r15\n"
  "finit\n"
  "fldcw -0x8(%rbp)\n"
  "fstpl -0x10(%rbp)\n"
  "movq -0x10(%rbp), %xmm0\n"
  "popq %r15\n"
  "popq %r14\n"
  "popq %r13\n"
  "popq %r12\n"
  "popq %rbx\n"
  "fldcw -0x6(%rbp)\n"
  "ldmxcsr -0x4(%rbp)\n"
  "movq %rbp, %rsp\n"
  "popq %rbp\n"
  "retq";
  try {
    buf = assem.assemble(str);
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
