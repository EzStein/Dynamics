#include "parser.h"
#include "compiler/token.h"
#include <stack>
using std::istream;
using std::string;
using std::map;
using std::stack;

parser::parser(istream& _stream, const map<string, token>& lexerDef, unsigned char const * const _opArr):
opArr(_opArr), lex(_stream, lexerDef) {

}

void parser::parse() {
  stack<token> tokenStack;
  tokenStack.push(token::ENDPOINT);
  while(1) {
    string lexeme;
    token next = lex.peek(lexeme);
    token top = tokenStack.top();
    precedence prec = get_precedence(top, next);
    if(top == token::ENDPOINT && next == token::ENDPOINT) {
      return;
    } else if(prec == precedence::LESS_THAN) {
      next = lex.next_token(lexeme);
      tokenStack.push(next);
    } else if(prec == precedence::GREATER_THAN) {
      while(1) {
        next = top;
        tokenStack.pop();
        top = tokenStack.top();
        if(get_precedence(top, next) == precedence::LESS_THAN)
          break;
      }

    } else {
      throw "PARSING ERROR";
    }
  }


}

precedence parser::get_precedence(token tk1, token tk2) const {
  return ::get_precedence(opArr, tk1, tk2);
}
