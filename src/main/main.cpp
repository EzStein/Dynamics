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



  stringstream sstream("(3)2(2)22(4)");
  parser parse(sstream);
  AST ast(parse.parse());
  std::cout << ast << std::endl;
  std::cout << ast.evaluate() << std::endl;
  std::cout << 3 - 4 * -2 << std::endl;

  return 0;
}
