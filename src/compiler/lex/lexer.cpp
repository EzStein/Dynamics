#include <stdexcept>
#include <utility>
#include "compiler/lex/lexer.h"

using std::map;
using std::istream;
using std::string;
using std::invalid_argument;

lexer::lexer() {

}

lexer::lexer(istream* _stream, const map<string, token>& lexerDef) : stream(_stream), previousToken(token::ERROR) {

  if(lexerDef.empty())
    throw invalid_argument("To define a lexer, you need at least one rule!");

  /*Initialize the NFA with one start state*/
  fa.start_state = fa.add_state();

  /*Iterate over each key-value pair in the mapping of rules*/
  typedef map<string, token>::const_iterator iterator;
  for(iterator iter = lexerDef.begin(); iter != lexerDef.end(); ++iter) {
    /*Create a temporary nfa that matches the rule's regular expression*/
    NFA tmp(iter->first);

    /*Record the original size of our nfa*/
    state_type originalSize = fa.size();

    /*We incorporate the temporary nfa into our own. The states of the temporary nfa now have
    id's exactly originalSize greater*/
    fa.augment_table(tmp);

    /*We make the (single) accepting state of the tmp nfa an accepting state of our new nfa as well*/
    fa.accepting_states.insert(*tmp.accepting_states.begin() + originalSize);

    /*Add an epsilon transition from the start state of our nfa to the start state of the added one*/
    fa.add_transition(fa.start_state,'\x00',tmp.start_state + originalSize);

    /*The newly added accepting state corresponds to the token of the rule*/
    tokenMap[*tmp.accepting_states.begin() + originalSize] = iter->second;
  }



  /*Compute the initial nextToken and nextLexeme values*/
  set_next_token();
}


token lexer::next_token(std::string& lexeme) {
  /*Set the return values to what would be returned by a call to peek*/
  lexeme = nextLexeme;
  token ret = nextToken;

  /*Set the previous values*/
  previousToken = ret;
  previousLexeme = lexeme;

  /*Set the next token and lexeme*/
  set_next_token();

  return ret;
}

void lexer::set_next_token() {
  currPos = stream->tellg();

  /*Now compute the next return values*/
  state_collection_type states = fa.accept_longest_prefix(*stream, nextLexeme, true);

  /*If an empty state was returned, either we have reached the end of file, or there was no matching prefix*/
  if(states.empty()) {
    if(stream->peek(), stream->eof())
      nextToken = token::ENDPOINT;
    else
      nextToken = token::ERROR;
  } else {
    /*Gets the smallest accepting state which is also the first one added*/
    nextToken = tokenMap[*states.begin()];
  }

  if(nextToken == token::MINUS && (::is_operator(previousToken) ||
    previousToken == token::ERROR /*FOR START OF STREAM*/ || previousToken == token::LEFT_PAREN)) {
      nextToken = token::UNARY_MINUS;
  }

  if(nextToken == token::LEFT_PAREN && (!::is_operator(previousToken) && previousToken != token::LEFT_PAREN && previousToken != token::ERROR /*FOR STREAM START*/)) {
    nextToken = token::ASTERISK;
    stream->seekg(currPos);
    /*Previous token remains unchanged*/
  } else if(previousToken == token::RIGHT_PAREN && (!::is_operator(nextToken) && nextToken != token::RIGHT_PAREN && nextToken != token::ENDPOINT)) {
    nextToken = token::ASTERISK;
    stream->seekg(currPos);
  } else if(nextToken == token::ID && (previousToken == token::ID || previousToken == token::NUMBER)) {
    nextToken = token::ASTERISK;
    stream->seekg(currPos);
  } else if(previousToken == token::ID && nextToken == token::NUMBER) {
    nextToken = token::ASTERISK;
    stream->seekg(currPos);
  }
}

token lexer::peek(std::string& lexeme) {
  lexeme = nextLexeme;
  return nextToken;
}

token lexer::previous(std::string& lexeme) {
  lexeme = previousLexeme;
  return previousToken;
}
