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
  /*A stack containing any grammer symbol. Nonterminals
  on this stack hold a pointer to the node representing that area of the tree*/
  stack<grammar_symbol> symbolStack;

  /*A stack containing only terminals*/
  /*stack<token> terminalStack;
  terminalStack.push(token::ENDPOINT);
  symbolStack.push(grammar_symbol{token::ENDPOINT, NULL});
  while(1) {
    string lexeme;
    token next = lex.peek(lexeme);
    token top = terminalStack.top();
    precedence prec = get_precedence(top, next);

    if(top == token::ENDPOINT && next == token::ENDPOINT) {
      return;
    } else if(prec == precedence::LESS_THAN) {
      lex.next_token(lexeme);
      terminalStack.push(next);
      symbolStack.push(grammar_symbol{next, NULL});
    } else if(prec == precedence::GREATER_THAN) {
      while(1) {
        next = top;
        terminalStack.pop();
        top = terminalStack.top();
        if(get_precedence(top, next) == precedence::LESS_THAN)
          break;
      }
      node* nodePtr = new node;
      //INITIALIZE NODE

      while(symbolStack.top().tok != top) {
        if(symbolStack.top().tok == token::NONTERMINAL) {
          if(!nodePtr->rightChild)
            nodePtr->rightChild = symbolStack.top().nodePtr;
          else if(!nodePtr->leftChild)
            nodePtr->leftChild = symbolStack.top().nodePtr;
        }
        symbolStack.pop();
      }
      symbolStack.push(grammar_symbol{token::NONTERMINAL, nodePtr});

    } else {
      throw "PARSING ERROR";
    }
  }*/


}

precedence parser::get_precedence(token tk1, token tk2) const {
  return ::get_precedence(opArr, tk1, tk2);
}
