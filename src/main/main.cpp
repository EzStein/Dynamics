#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <map>
#include <cassert>

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
    stringstream sstream(str);
    parser parse(sstream);
    AST ast(parse.parse());
    std::cout << "Writing to file test.out\n\n";
    unsigned int size = ast.code_size();
    unsigned char * buf = new unsigned char[size];
    ast.emit_code(std::cout, buf) << std::endl;
    ofstream handle;
    handle.open("test.out", ios::binary | ios::out);
    handle.write(reinterpret_cast<const char *>(buf), size);
    handle.close();
    break;
  }
  return 0;
}
