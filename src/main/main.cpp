#include <iostream>
#include <set>
#include "compiler/NFA.h"
#include "compiler/DFA.h"
#include <sstream>
#include <map>
#include <cassert>
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
  /*unsigned char * arr = new unsigned char[static_cast<unsigned int>(token::TOKEN_COUNT) * CHUNK_SIZE];
  for(int i = 0; i != static_cast<int>(token::TOKEN_COUNT); ++i) {
    for(int j = 0; j != static_cast<int>(token::TOKEN_COUNT); ++j) {
      set_precedence(arr, static_cast<token>(i), static_cast<token>(j), static_cast<precedence>((0)%3));
    }
  }
  for(int i = 0; i != static_cast<int>(token::TOKEN_COUNT); ++i) {
    for(int j = 0; j != static_cast<int>(token::TOKEN_COUNT); ++j) {
      cout << i << ", " << j <<  endl;
      assert(get_precedence(arr, static_cast<token>(i), static_cast<token>(j))== static_cast<precedence>((0)%3));
    }
  }*/
  //delete[] arr;
  map<string, token> lexDef;
  //lexDef[string("[0123456789][0123456789]*\\.[0123456789][0123456789]*")] = token::PLUS;
  //lexDef[string("[abcdefghi][abcdefghi]*")] = token::ID;
  lexDef[string("\\d\\d*")] = token::ID;
  lexDef[string("\\a\\a*")] = token::ID;
  stringstream sstream("123abc2234");
  lexer lex(sstream, lexDef);
  string lexeme;
  token tok;
  while((tok = lex.next_token(lexeme)) != (token::ENDPOINT)) {
    cout << "TOKEN: " << tok << " LEXEME: " << lexeme << endl;
  }
  return 0;
}
