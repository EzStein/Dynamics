#include <stdexcept>
#include <utility>
#include "compiler/lexer.h"

using std::map;
using std::istream;
using std::string;
using std::invalid_argument;

lexer::lexer(istream& _stream, const map<string, token>& lexerDef) : stream(_stream) {


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

  /*Now compute the next return values*/
  state_collection_type states = fa.accept_longest_prefix(stream, nextLexeme);

  /*If an empty state was returned, either we have reached the end of file, or there was no matching prefix*/
  if(states.empty()) {
    if(stream.peek(), stream.eof())
      nextToken = token::ENDPOINT;
    else
      nextToken = token::ERROR;
  } else {
    /*Gets the smallest accepting state which is also the first one added*/
    nextToken = tokenMap[*states.begin()];
  }
}


token lexer::next_token(std::string& lexeme) {
  /*Set the return values to what would be returned by a call to peek*/
  lexeme = nextLexeme;
  token ret = nextToken;

  /*Now compute the next return values*/
  state_collection_type states = fa.accept_longest_prefix(stream, nextLexeme);

  /*If an empty state was returned, either we have reached the end of file, or there was no matching prefix*/
  if(states.empty()) {
    if(stream.peek(), stream.eof())
      nextToken = token::ENDPOINT;
    else
      nextToken = token::ERROR;
  } else {
    /*Gets the smallest accepting state which is also the first one added*/
    nextToken = tokenMap[*states.begin()];
  }


  return ret;
}

token lexer::peek(std::string& lexeme) {
  lexeme = nextLexeme;
  return nextToken;
}
