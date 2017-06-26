#include "parser.h"
#include "compiler/token.h"


#include "compiler/ast/AST.h"
#include "compiler/ast/expression_node.h"
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/plus_operator_node.h"
#include "compiler/ast/unary_minus_operator_node.h"
#include "compiler/ast/binary_minus_operator_node.h"
#include "compiler/ast/multiply_operator_node.h"
#include "compiler/ast/divide_operator_node.h"
#include "compiler/ast/factorial_operator_node.h"
#include "compiler/ast/variable_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"

#include <stack>
#include <stdexcept>
#include <iostream>
#include <deque>
#include <algorithm>
#include <cassert>
using std::istream;
using std::string;
using std::map;
using std::stack;
using std::invalid_argument;
using std::deque;

parser::parser(istream& _stream, const map<string, token>& lexerDef, unsigned char const * const _opArr):
opArr(_opArr), lex(_stream, lexerDef) {

}

/*Copies stack*/
bool verify_invariant(stack<grammar_symbol> symbolStack, stack<token> terminalStack) {
  /*std::cout << "AaaaaaAAA" << std::endl;
  typedef deque<grammar_symbol>::const_iterator symIter;
  typedef deque<token>::const_iterator termIter;
  deque<grammar_symbol> deqSymbolStack;
  stack<grammar_symbol> symbolStackNew(deqSymbolStack);
  std::swap(symbolStackNew, symbolStack);

  deque<token> deqTerminalStack;
  stack<token> terminalStackNew(deqTerminalStack);
  std::swap(terminalStackNew, terminalStack);

  symIter symIterA = deqSymbolStack.begin();
  termIter termIterA = deqTerminalStack.begin();

  for(;termIterA != deqTerminalStack.end(); ++termIterA) {
    while(symIterA->tok == token::NONTERMINAL) {
      ++symIterA;
    }
    if(symIterA->tok != *termIterA)
      return false;

    std::cout << symIterA->tok << ", " << *termIterA << std::endl;
    ++symIterA;
  }
  return true;*/


  while(terminalStack.size() != 0) {

    while(symbolStack.top().tok == token::NONTERMINAL) {
      symbolStack.pop();
    }
    if(terminalStack.top() != symbolStack.top().tok) return false;
    symbolStack.pop();
    terminalStack.pop();
  }
  return true;
}

AST parser::parse() {
  AST ast(200000);
  /*A stack containing only nonterminals that point to a node in the AST tree*/
  stack<expression_node*> symbolStack;

  /*A stack containing only the terminals in the same order as the terminals on the symbolStack*/
  stack<token> terminalStack;
  terminalStack.push(token::ENDPOINT);
  /*
  * INVARIANT: terminalStack contains only the terminals in the symbolStack and in the same order
  * that the terminals appear in on the symbol stack.
  */
  while(1) {

    string lexeme;
    token next = lex.peek(lexeme);
    token top = terminalStack.top();
    precedence prec = get_precedence(top, next);
    std::cout << top << "  "<< static_cast<int>(prec)<< std::endl;
    if(top == token::ENDPOINT && next == token::ENDPOINT) {
      ast.set_root(symbolStack.top());
      return ast;
    } else if(prec != precedence::GREATER_THAN) {
      next = lex.next_token(lexeme);
      terminalStack.push(next);
    } else if(prec == precedence::GREATER_THAN) {
      next = terminalStack.top();
      while(1) {
        terminalStack.pop();
        top = terminalStack.top();
        if(get_precedence(top, next) == precedence::LESS_THAN)
          break;

        next = top;
      }

      expression_node* nodePtr = nullptr;
      //INITIALIZE NODE
      expression_node* rightChild = nullptr, *leftChild = nullptr;


      /*The terminal that was removed*/
      switch(next) {
        case token::ID:
          nodePtr = ast.make_variable_leaf_node(1);
          symbolStack.push(nodePtr);
          break;
        case token::EXCLAMATION:
          rightChild = symbolStack.top();
          symbolStack.pop();
          nodePtr = ast.make_unary_operator_node<factorial_operator_node>(rightChild);
          symbolStack.push(nodePtr);
          break;
        case token::CARET:
          rightChild = symbolStack.top();
          //POP the nonterminal
          symbolStack.pop();
          leftChild = symbolStack.top();
          //POP the nonterminal
          symbolStack.pop();
          nodePtr = ast.make_binary_operator_node<exponentiation_operator_node>(leftChild, rightChild);
          symbolStack.push(nodePtr);
          break;
        case token::UNARY_MINUS:
            rightChild = symbolStack.top();
            symbolStack.pop();
            nodePtr = ast.make_unary_operator_node<unary_minus_operator_node>(rightChild);
            symbolStack.push(nodePtr);
          break;
        case token::ASTERISK:
          rightChild = symbolStack.top();
          //POP the nonterminal
          symbolStack.pop();

          leftChild = symbolStack.top();
          //POP the nonterminal
          symbolStack.pop();
            nodePtr = ast.make_binary_operator_node<multiply_operator_node>(leftChild, rightChild);
            symbolStack.push(nodePtr);
          break;
        case token::FORWARD_SLASH:
            rightChild = symbolStack.top();
            //POP the nonterminal
            symbolStack.pop();

            leftChild = symbolStack.top();
            //POP the nonterminal
            symbolStack.pop();
            nodePtr = ast.make_binary_operator_node<divide_operator_node>(leftChild, rightChild);
            symbolStack.push(nodePtr);
          break;
        case token::PLUS:
            rightChild = symbolStack.top();
            //POP the nonterminal
            symbolStack.pop();

            leftChild = symbolStack.top();
            //POP the nonterminal
            symbolStack.pop();
            nodePtr = ast.make_binary_operator_node<plus_operator_node>(leftChild, rightChild);
            symbolStack.push(nodePtr);
          break;
        case token::MINUS:
            rightChild = symbolStack.top();
            //POP the nonterminal
            symbolStack.pop();

            leftChild = symbolStack.top();
            //POP the nonterminal
            symbolStack.pop();
            nodePtr = ast.make_binary_operator_node<binary_minus_operator_node>(leftChild, rightChild);
            symbolStack.push(nodePtr);
          break;
        case token::RIGHT_PAREN:
          throw invalid_argument("THIS SHOULD NEVER OCCUR");
          break;
        case token::LEFT_PAREN:
          //We do nothing
          break;
        default:
          std::cout << "DEFAULT: " << next << std::endl;
      }
    }
  }
}

precedence parser::get_precedence(token tk1, token tk2) const {
  return ::get_precedence(opArr, tk1, tk2);
}
