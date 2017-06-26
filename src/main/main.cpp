#include <iostream>
#include <set>
#include "compiler/NFA.h"
#include "compiler/DFA.h"
#include <sstream>
#include <map>
#include <cassert>
#include "compiler/token.h"
#include "compiler/lexer.h"
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
  stringstream sstream("ab324.3c4defg2a2");
  parser parse(sstream);
  AST ast(parse.parse());
  std::cout << ast << std::endl;
  std::cout << ast.evaluate() << std::endl;
  return 0;
}
