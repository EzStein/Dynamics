#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <map>
#include <cassert>
#include <sys/mman.h>

#include "compiler/ast/AST.h"
#include "compiler/parser.h"
using namespace std;

ostream& operator<<(ostream& out, const set<unsigned int>& myset) {
  out << '{';
  for(set<unsigned int>::const_iterator i = myset.begin(); i!=myset.end(); ++i) {
    out << *i << ", ";
  }
  out << '}';
  return out;
}

int main() {

  while(1) {
    cout << "Enter an expression to compile: " << flush;
    string str;

    getline(cin, str);
    std::cout << "A"  << std::endl;
    stringstream sstream(str);
    parser parse(sstream);
    std::cout << "B" << std::endl;

    AST ast(parse.parse());
    std::cout << "C" << std::endl;
    std::cout << "Writing to file test.out\n\n" << std::endl;
    unsigned int size = ast.code_size();
    unsigned char * buf = reinterpret_cast<unsigned char *>(mmap(NULL, size,
      PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    ast.emit_code(std::cout, buf) << std::endl;
    ofstream handle;
    handle.open("test.out", ios::binary | ios::out);
    handle.write(reinterpret_cast<const char *>(buf), size);
    handle.close();

    double (*func)(double) = 0;

    *reinterpret_cast<void**>(&func) = buf;
    std::cout << "The result is: " << func(3) << "\n";
    munmap(buf, size);
  }
  return 0;
}
