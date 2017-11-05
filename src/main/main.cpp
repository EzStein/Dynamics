#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <set>
#include <sstream>
#include <map>
#include <cassert>
#include <sys/mman.h>
#include <string>
#include "math/matrix.h"

#include "math/vector.h"

#include "compiler/ast/AST.h"
#include "compiler/parser.h"
#include "compiler/front/driver.h"
#include "compiler/asm/assembler.h"
using namespace std;

AST compile_as_tree(const std::string& str) {
  stringstream sstream(str);
  parser parse(sstream);

  list<symbol> symbolTable;
  /*Compile the function*/
  return AST(parse.parse(symbolTable));
}

int main() {
  /*std::string str("z+1*x+y");
  stringstream sstream(str);
  parser parse(sstream);

  list<symbol> symbolTable;
  /*Compile the function*/
  /*AST ast(parse.parse(symbolTable));
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

  return 0;*/

  //math::matrix<double, 3, 3> mat(0,1,2,3,4,5,6,7,8);

  /*constexpr int size = 4;
  math::matrix<double,size,size> mat;
  for(int i = 0; i != size; ++i) {
    for(int j = 0; j != size; ++j) {
      mat[i][j] = (i << 2)/(j+i+1);
    }
  }

  math::matrix<double,4,4> mat2(0,1,2,23,4,5,6,7,8,9,10,11,12,13,34,2);

  //std::cout << std::setprecision(std::numeric_limits<double>::digits10 + 1);
  std::cout << std::setprecision(20);
  std::cout << mat2 << "\n\n" << std::endl;
  std::cout << mat2.determinant() << std::endl;
  //std::cout << 0.010101010101e-35 << std::endl;*/

  math::matrix<AST, 2, 2> mat(compile_as_tree("1-x"),AST(1),AST(2),AST(3));
  std::cout << mat << std::endl;
  //mat = invert(mat)*mat;
  //std::cout << mat << std::endl;
  mat = math::matrix<AST,2,2>(mat[0][0].simplify(),
          mat[0][1].simplify(),
        mat[1][0].simplify(),
        mat[1][1].simplify());

  std::cout << mat << std::endl;

  return 0;
}
