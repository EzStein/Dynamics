#include "parser.h"
#include "compiler/token.h"
#include <stack>
using std::istream;
using std::string;
using std::map;
using std::stack;

parser::parser(istream& _stream, const map<string, token>& lexerDef): stream(_stream), lex(_stream, lexerDef) {

}

void parser::parse() {
  //stack<token> tokenStack;
  //token tok = lex.next_token();
}
