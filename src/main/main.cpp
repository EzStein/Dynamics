#include <iostream>
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
    std::cout << "Here is the compilation:\n";
    ast.emit_code(std::cout) << std::endl;
  }
  return 0;
}
