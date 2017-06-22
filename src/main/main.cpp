#include <iostream>
#include <set>
#include "compiler/NFA.h"
#include "compiler/DFA.h"
#include <sstream>
#include <map>
#include "compiler/token.h"
#include "compiler/lexer.h"
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
  map<string, token> lexDef;
  lexDef[string("(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*.(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*")] = token::PLUS;
  lexDef[string("(a|b|c|d|e|f|g|h|i)(a|b|c|d|e|f|g|h|i)*")] = token::ID;

  stringstream sstream("abc123.2234");
  lexer lex(sstream, lexDef);
  string lexeme;
  token tok;
  while((tok = lex.next_token(lexeme)) != token::ERROR) {
    cout << "TOKEN: " << tok << " LEXEME: " << lexeme << endl;

  }
  return 0;
}
