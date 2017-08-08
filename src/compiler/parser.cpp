#include "parser.h"
#include "util/util.h"
#include "compiler/lex/token.h"
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

#include <list>
#include <stack>
#include <stdexcept>
#include <iostream>
#include <deque>
#include <algorithm>
#include <cassert>
using std::list;
using std::istream;
using std::string;
using std::map;
using std::stack;
using std::invalid_argument;
using std::deque;

parser::parser(istream& stream) {
  /*Construct lexer*/
  map<string, token> lexDef;
  lexDef[string("\\s*\\(\\s*")] = token::LEFT_PAREN;
  lexDef[string("\\s*\\)\\s*")] = token::RIGHT_PAREN;
  lexDef[string("\\s*+\\s*")] = token::PLUS;
  lexDef[string("\\s*-\\s*")] = token::MINUS;
  lexDef[string("\\s*/\\s*")] = token::FORWARD_SLASH;
  lexDef[string("\\s*\\*\\s*")] = token::ASTERISK;
  lexDef[string("\\s*!\\s*")] = token::EXCLAMATION;
  lexDef[string("\\s*^\\s*")] = token::CARET;
  lexDef[string("\\s*\\d\\d*|\\d\\d*\\.\\d\\d*\\s*")] = token::NUMBER;
  lexDef[string("\\s*\\a\\s*")] = token::ID;
  lex = lexer(&stream, lexDef);

  /*Generate operator array*/
  opArr = new unsigned char[static_cast<int>(token::TOKEN_COUNT)*CHUNK_SIZE];

  set_precedence(token::NUMBER, token::ID, precedence::EQUAL_TO);
  set_precedence(token::NUMBER, token::LEFT_PAREN, precedence::EQUAL_TO);
  set_precedence(token::NUMBER, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::CARET, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::NUMBER, token::NUMBER, precedence::EQUAL_TO);

  set_precedence(token::ID, token::ID, precedence::EQUAL_TO);
  set_precedence(token::ID, token::LEFT_PAREN, precedence::EQUAL_TO);
  set_precedence(token::ID, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::ID, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(token::ID, token::CARET, precedence::GREATER_THAN);
  set_precedence(token::ID, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(token::ID, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::ID, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::ID, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::ID, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::ID, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::ID, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::ID, token::NUMBER, precedence::LESS_THAN);

  set_precedence(token::LEFT_PAREN, token::ID, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::RIGHT_PAREN, precedence::EQUAL_TO);
  set_precedence(token::LEFT_PAREN, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::CARET, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::PLUS, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::MINUS, precedence::LESS_THAN);
  set_precedence(token::LEFT_PAREN, token::ENDPOINT, precedence::EQUAL_TO);
  set_precedence(token::LEFT_PAREN, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::RIGHT_PAREN, token::ID, precedence::EQUAL_TO);
  set_precedence(token::RIGHT_PAREN, token::LEFT_PAREN, precedence::EQUAL_TO);
  set_precedence(token::RIGHT_PAREN, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::CARET, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::RIGHT_PAREN, token::NUMBER, precedence::LESS_THAN);

  set_precedence(token::EXCLAMATION, token::ID, precedence::LESS_THAN);
  set_precedence(token::EXCLAMATION, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::EXCLAMATION, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::EXCLAMATION, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::CARET, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::EXCLAMATION, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::CARET, token::ID, precedence::LESS_THAN);
  set_precedence(token::CARET, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::CARET, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::CARET, token::CARET, precedence::LESS_THAN);
  set_precedence(token::CARET, token::UNARY_MINUS, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::CARET, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::UNARY_MINUS, token::ID, precedence::LESS_THAN);
  set_precedence(token::UNARY_MINUS, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::UNARY_MINUS, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::UNARY_MINUS, token::CARET, precedence::LESS_THAN);
  set_precedence(token::UNARY_MINUS, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::UNARY_MINUS, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::UNARY_MINUS, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::ASTERISK, token::ID, precedence::LESS_THAN);
  set_precedence(token::ASTERISK, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::ASTERISK, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::ASTERISK, token::CARET, precedence::LESS_THAN);
  set_precedence(token::ASTERISK, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::ASTERISK, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::ASTERISK, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::FORWARD_SLASH, token::ID, precedence::LESS_THAN);
  set_precedence(token::FORWARD_SLASH, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::FORWARD_SLASH, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::FORWARD_SLASH, token::CARET, precedence::LESS_THAN);
  set_precedence(token::FORWARD_SLASH, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::FORWARD_SLASH, token::ASTERISK, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::FORWARD_SLASH, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::FUNCTION, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::FORWARD_SLASH, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::FUNCTION, token::ID, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::FUNCTION, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::CARET, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(token::FUNCTION, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::FUNCTION, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::FUNCTION, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::FUNCTION, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::PLUS, token::ID, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::PLUS, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::CARET, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(token::PLUS, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::PLUS, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::PLUS, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::PLUS, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::MINUS, token::ID, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::RIGHT_PAREN, precedence::GREATER_THAN);
  set_precedence(token::MINUS, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::CARET, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(token::MINUS, token::PLUS, precedence::GREATER_THAN);
  set_precedence(token::MINUS, token::MINUS, precedence::GREATER_THAN);
  set_precedence(token::MINUS, token::ENDPOINT, precedence::GREATER_THAN);
  set_precedence(token::MINUS, token::NUMBER, precedence::LESS_THAN);


  set_precedence(token::ENDPOINT, token::ID, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::LEFT_PAREN, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::RIGHT_PAREN, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::EXCLAMATION, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::CARET, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::UNARY_MINUS, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::ASTERISK, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::FORWARD_SLASH, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::FUNCTION, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::PLUS, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::MINUS, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::ENDPOINT, precedence::LESS_THAN);
  set_precedence(token::ENDPOINT, token::NUMBER, precedence::LESS_THAN);
}

precedence parser::get_precedence(token tk1, token tk2) {
  return static_cast<precedence>((opArr[static_cast<unsigned int>(tk1) * CHUNK_SIZE
  + static_cast<unsigned int>(tk2)/4] & (0xC0u >> 2*(static_cast<unsigned int>(tk2)%4))) >> 2*(4-(static_cast<unsigned int>(tk2)%4 + 1)));
}

void parser::set_precedence(token tk1, token tk2, precedence prec) {
  unsigned char msk = ~(0xC0u >> 2*(static_cast<unsigned int>(tk2)%4));
  unsigned char val = static_cast<unsigned char>(
    static_cast<unsigned int>(prec) << 2*(4 - (static_cast<unsigned int>(tk2)%4 + 1)));

  /*Clear away the bits that were previously there*/
  opArr[static_cast<unsigned int>(tk1) * CHUNK_SIZE + static_cast<unsigned int>(tk2)/4] &= msk;

  /*Set the appropriate bits*/
  opArr[static_cast<unsigned int>(tk1) * CHUNK_SIZE + static_cast<unsigned int>(tk2)/4] |= val;
}

parser::~parser() {
  delete[] opArr;
}

AST parser::parse() {
  list<symbol> symbolTable;

  /*A stack containing all the node pointers.
  Each node represents a symbol (terminal or nonterminal) in the grammar*/
  stack<expression_node*> nodeStack;

  /*A stack containing only the terminals and the special token ENDPOINT*/
  stack<token> terminalStack;
  terminalStack.push(token::ENDPOINT);


  while(1) {
    /*Retrieve the next token without removing it*/
    string lexeme;
    token next = lex.peek(lexeme);

    /*Compare the next token with the token on the top of the terminal stack*/
    token top = terminalStack.top();
    precedence prec = get_precedence(top, next);

    /*We are done when we have reached the end of the stream and the terminal stack contains just the endpoint*/
    if(top == token::ENDPOINT && next == token::ENDPOINT) {
      AST ast(nodeStack.top());
      return ast;
    }
    if(prec != precedence::GREATER_THAN) {
      /*If the precedence is not greator, we consume the token and add it to the stack*/
      terminalStack.push(lex.next_token(lexeme));
      continue;
    }

    /*Otherwise the precedence is greater, and things get complicated...*/
    /*We now ignore the token we just peeked at.
    We backtrack, removing tokens from the stack until we encounter a LESS_THAN precedence.
    Next is set to the token on the top of the terminal stack*/
    lex.previous(lexeme);
    next = terminalStack.top();
    while(1) {
      terminalStack.pop();
      top = terminalStack.top();
      if(get_precedence(top, next) == precedence::LESS_THAN)
        break;

      next = top;
    }
    /*Next is the last removed token (terminal)*/

    expression_node* nodePtr = nullptr;
    //INITIALIZE NODE
    expression_node* rightChild = nullptr, *leftChild = nullptr;


    /*We prepare the creation of a symbol by stripping whitespace from the lexeme*/
    util::strip_white_space(lexeme);


    /*The terminal that was removed*/
    switch(next) {
      case token::ID: {
        /*Holds the id of the next unique symbol*/
        static int symbolId = -1;
        /*We encounter an identifier, so we add it to the symbol table if does not yet exist and Construct
        a leaf node with a pointer to that symbol.*/
        symbol sym(lexeme);
        /*True if the list does not contain the symbol*/
        symbol::ptr_type symPtr = std::find(symbolTable.begin(), symbolTable.end(), sym);


        /*In the case that the symbol does not yet exist, we add it*/
        if(symPtr == symbolTable.end()) {
          /*We add the symbol to the list*/
          sym.id = ++symbolId;
          symbolTable.push_back(sym);
          /*The table is now nonempty. We get a pointer/iterator to the last element
          by decrementing end() by 1*/
          symPtr = symbolTable.end();
          --symPtr;
        }
        nodePtr = AST::make_variable_leaf_node(symPtr);
        nodeStack.push(nodePtr);
        }
        break;
      case token::NUMBER:
        nodePtr = AST::make_number_leaf_node(util::string_to_double(lexeme));
        nodeStack.push(nodePtr);
        break;
      case token::EXCLAMATION:
        rightChild = nodeStack.top();
        nodeStack.pop();
        nodePtr = AST::make_unary_operator_node<factorial_operator_node>(rightChild);
        nodeStack.push(nodePtr);
        break;
      case token::CARET:
        rightChild = nodeStack.top();
        //POP the nonterminal
        nodeStack.pop();
        leftChild = nodeStack.top();
        //POP the nonterminal
        nodeStack.pop();
        nodePtr = AST::make_binary_operator_node<exponentiation_operator_node>(leftChild, rightChild);
        nodeStack.push(nodePtr);
        break;
      case token::UNARY_MINUS:
          rightChild = nodeStack.top();
          nodeStack.pop();
          nodePtr = AST::make_unary_operator_node<unary_minus_operator_node>(rightChild);
          nodeStack.push(nodePtr);
        break;
      case token::ASTERISK:
        rightChild = nodeStack.top();
        //POP the nonterminal
        nodeStack.pop();

        leftChild = nodeStack.top();
        //POP the nonterminal
        nodeStack.pop();
          nodePtr = AST::make_binary_operator_node<multiply_operator_node>(leftChild, rightChild);
          nodeStack.push(nodePtr);
        break;
      case token::FORWARD_SLASH:
          rightChild = nodeStack.top();
          //POP the nonterminal
          nodeStack.pop();

          leftChild = nodeStack.top();
          //POP the nonterminal
          nodeStack.pop();
          nodePtr = AST::make_binary_operator_node<divide_operator_node>(leftChild, rightChild);
          nodeStack.push(nodePtr);
        break;
      case token::PLUS:
          rightChild = nodeStack.top();
          //POP the nonterminal
          nodeStack.pop();

          leftChild = nodeStack.top();
          //POP the nonterminal
          nodeStack.pop();
          nodePtr = AST::make_binary_operator_node<plus_operator_node>(leftChild, rightChild);
          nodeStack.push(nodePtr);
        break;
      case token::MINUS:
          rightChild = nodeStack.top();
          //POP the nonterminal
          nodeStack.pop();

          leftChild = nodeStack.top();
          //POP the nonterminal
          nodeStack.pop();
          nodePtr = AST::make_binary_operator_node<binary_minus_operator_node>(leftChild, rightChild);
          nodeStack.push(nodePtr);
        break;
      case token::RIGHT_PAREN:
        throw invalid_argument("THIS SHOULD NEVER OCCUR: RIGHT_PAREN");
        break;
      case token::LEFT_PAREN:
        //We do nothing
        break;
      default:
        throw invalid_argument("THIS SHOULD NEVER OCCUR: DEFAULT");
    }

  }
}
