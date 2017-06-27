#include <iostream>
#include <iomanip>
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

int mai2() {

  while(1) {
    cout << "Enter an expression to evaluate: " << flush;
    string str;
    getline(cin, str);
    stringstream sstream(str);
    parser parse(sstream);
    AST ast(parse.parse());
    cout << "Your expression evaluates to " << ast.evaluate() << endl;
  }
  return 0;
}
