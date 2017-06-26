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
  map<string, token> lexDef;
  //lexDef[string("[0123456789][0123456789]*\\.[0123456789][0123456789]*")] = token::PLUS;
  //lexDef[string("[abcdefghi][abcdefghi]*")] = token::ID;
  lexDef[string("\\(")] = token::LEFT_PAREN;
  lexDef[string("\\)")] = token::RIGHT_PAREN;
  lexDef[string("+")] = token::PLUS;
  lexDef[string("-")] = token::MINUS;
  lexDef[string("/")] = token::FORWARD_SLASH;
  lexDef[string("*")] = token::ASTERISK;
  lexDef[string("!")] = token::EXCLAMATION;
  lexDef[string("^")] = token::CARET;
  lexDef[string("\\d\\d*|\\d\\d*\\.\\d\\d*")] = token::ID;

  unsigned char * opArr = new_op_array();
  set_precedence(opArr, token::ID, token::ID, precedence::EQUAL_TO);
  set_precedence(opArr, token::ID, token::LEFT_PAREN, precedence::EQUAL_TO);
  set_precedence(opArr, token::ID, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::CARET, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::ID, token::ENDPOINT, precedence::GREATER_THAN);

  set_precedence(opArr, token::LEFT_PAREN, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::RIGHT_PAREN, precedence::EQUAL_TO);
  set_precedence(opArr, token::LEFT_PAREN, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::PLUS, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::LEFT_PAREN, token::ENDPOINT, precedence::EQUAL_TO);


  set_precedence(opArr, token::RIGHT_PAREN, token::ID, precedence::EQUAL_TO);
  set_precedence(opArr, token::RIGHT_PAREN, token::LEFT_PAREN, precedence::EQUAL_TO);
  set_precedence(opArr, token::RIGHT_PAREN, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::CARET, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::RIGHT_PAREN, token::ENDPOINT, precedence::GREATER_THAN);

  set_precedence(opArr, token::EXCLAMATION, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::CARET, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::EXCLAMATION, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::CARET, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::CARET, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::CARET, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::CARET, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::CARET, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::CARET, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::UNARY_MINUS, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::UNARY_MINUS, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::ASTERISK, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::ASTERISK, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::ASTERISK, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::ASTERISK, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::ASTERISK, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::ASTERISK, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::ASTERISK, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::ASTERISK, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::ASTERISK, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::ASTERISK, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::ASTERISK, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::ASTERISK, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::FORWARD_SLASH, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::FORWARD_SLASH, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::FUNCTION, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::FUNCTION, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(opArr, token::FUNCTION, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::FUNCTION, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::FUNCTION, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::PLUS, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::PLUS, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(opArr, token::PLUS, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::PLUS, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::PLUS, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::MINUS, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(opArr, token::MINUS, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(opArr, token::MINUS, token::PLUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::MINUS, token::MINUS, precedence::GREATER_THAN);
  set_precedence(opArr, token::MINUS, token::ENDPOINT, precedence::GREATER_THAN);


  set_precedence(opArr, token::ENDPOINT, token::ID, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::RIGHT_PAREN, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::CARET, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::PLUS, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::MINUS, precedence::LESS_THAN);
  set_precedence(opArr, token::ENDPOINT, token::ENDPOINT, precedence::LESS_THAN);







  stringstream sstream("1+(1+2)");
  parser parse(sstream, lexDef, opArr);
  AST ast(parse.parse());
  std::cout << ast << std::endl;
  //ast.evaluate();
  delete_op_array(opArr);
  return 0;
}
