#include <iterator>
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
#include <stack>
#include "compiler/regex/NFA.h"

using std::map;
using std::vector;
using std::ostream;
using std::endl;
using std::stringstream;
using std::char_traits;
using std::string;
using std::stack;
using std::invalid_argument;
using std::istream;
using std::streampos;
using std::set;

NFA::NFA(const string& str) {
  stringstream sstream(str);
  parse_expression(sstream, *this);
}

NFA::NFA(const char * str) {
  stringstream sstream(str);
  parse_expression(sstream, *this);
}

NFA::NFA(const vector<map<char, state_collection_type > > & table) {
  this->table = table;
}

const state_collection_type& NFA::get_accepting_states() const {
  return accepting_states;
}

bool NFA::accepts(const char * string) const {
  state_collection_type set = extended_transition_function(start_state, string);
  return intersects(set.begin(), set.end(), accepting_states.begin(), accepting_states.end());
}

state_collection_type NFA::accept_longest_prefix(istream& stream, string& prefix, bool ignoreWhitespace) const {

  /*If nothing is matched, the the string containing the null character is returned*/
  prefix = string("\x00", 1);
  stringstream prefixBuilder;
  state_collection_type prefixStates;
  state_collection_type states;
  streampos prefixPosition = -1;
  states.insert(start_state);

  states = epsilon_closure(states);

  /*This is true if the NFA accepts the empty string*/
  if(intersects(accepting_states.begin(), accepting_states.end(), states.begin(), states.end())) {
    prefix = "";
    prefixStates = states;
    prefixPosition = stream.tellg();
  }


  char c;
  while(!stream.get(c).eof()) {

    /*Expand over transition function and epsilon closure*/
    states = transition_function(states, c);
    states = epsilon_closure(states);
    prefixBuilder.put(c);


    /*This prefix would be accepted*/

    if(states.empty()) {
      /*No prefix was recovered*/
      if(prefixPosition == -1) return state_collection_type();
      stream.seekg(prefixPosition);
      return intersect(prefixStates, accepting_states);
    }


    if(intersects(accepting_states.begin(), accepting_states.end(), states.begin(), states.end())) {
      prefix = prefixBuilder.str();
      prefixStates = states;
      prefixPosition = stream.tellg();
    }
  }



  /*When an end of file occurs, the fail bit will always be set. We unset it as well as the eof bit. In c++11
  seekg will always clear the eof bit. At this point we can check that seek g did nothing with the following expression
  *
  * if(stream.peek(), stream.eof())
  * this is exactly what we do in the lexer.
  */
  stream.clear();
  if(prefixPosition != -1)
    stream.seekg(prefixPosition);
  return intersect(prefixStates, accepting_states);
}

state_collection_type NFA::extended_transition_function(state_type state, const char * string) const {
  state_collection_type initial;
  initial.insert(state);

  /*Iterate over each character*/
  for(const char * current_char = string; *current_char != '\x00'; ++current_char) {

    /*Expand the initial set to include equivalent states over epsilon transitions*/
    initial = epsilon_closure(initial);

    /*For each state in initial, we add to reachable the reachable states through the given character*/
    initial = transition_function(initial, *current_char);

    /*We reached a nonrecoverable set of states*/
    if(initial.empty())
      return initial;
  }

  /*Expand one last time over the initial set to include equivalent states over epsilon transitions*/
  initial = epsilon_closure(initial);
  return initial;
}


state_collection_type NFA::epsilon_closure(state_collection_type initStates) const {
  stack<state_type> processingStack;
  state_collection_type processed;
  for(state_collection_type::const_iterator elem = initStates.begin(); elem != initStates.end(); ++elem) {
    processingStack.push(*elem);
    processed.insert(*elem);
  }

  while(!processingStack.empty()) {
    state_type state = processingStack.top();
    processingStack.pop();
    state_collection_type reachableStates = transition_function(state, '\x00');
    /*Iterate over all states reachable from the current one through epsilon transitions*/
    for(state_collection_type::const_iterator elem = reachableStates.begin(); elem != reachableStates.end(); ++elem) {

      // If the state is not in the processed collection
      if(processed.find(*elem) == processed.end()) {
        //Add it to the collection, and to the stack
        processed.insert(*elem);
        processingStack.push(*elem);
      }
    }
  }
  return processed;
}

state_collection_type NFA::epsilon_closure(state_type state) const {
  state_collection_type set;
  set.insert(state);
  return epsilon_closure(set);
}

state_collection_type NFA::transition_function(state_type state, char transition) const {
  map<char, state_collection_type> map = table[state];
  if(map.find(transition) == map.end()) {
    //The map does not contain the transition as a key, so we return an empty set
    return state_collection_type();
  } else {
    return map.at(transition);
  }
}


state_collection_type NFA::transition_function(state_collection_type states, char transition) const {
  state_collection_type ret;
  for(state_collection_type::const_iterator state = states.begin(); state != states.end(); ++state) {
    state_collection_type tmp = transition_function(*state, transition);
    copy(tmp.begin(), tmp.end(), inserter(ret, ret.begin()));
  }
  return ret;
}

state_type NFA::size() const {
  return table.size();
}

state_type NFA::add_state() {
  map<char, state_collection_type > empty;
  table.push_back(empty);
  return size() - 1;
}

void NFA::add_transition(state_type from_state, char transition_char, state_type to_state) {
  table[from_state][transition_char].insert(to_state);
}

void NFA::kleene_closure() {
  /*Add epsilon transition from accepting to start state*/
  add_transition(*accepting_states.begin(), '\x00', start_state);
  state_type new_start = add_state();
  state_type new_accepting = add_state();

  /*Add the appropriate epsilon transitions*/
  add_transition(new_start, '\x00', start_state);
  add_transition(*accepting_states.begin(), '\x00', new_accepting);
  add_transition(new_start, '\x00', new_accepting);
  start_state = new_start;

  accepting_states.erase(accepting_states.begin());
  accepting_states.insert(new_accepting);
}

void NFA::concatenation(const NFA& nfa) {
  state_type original_size = size();
  augment_table(nfa);
  add_transition(*accepting_states.begin(), '\x00', nfa.start_state + original_size);

  accepting_states.erase(accepting_states.begin());
  accepting_states.insert(*nfa.accepting_states.begin() + original_size);
}

void NFA::alternation(const NFA& nfa) {
  state_type original_size = size();
  augment_table(nfa);
  state_type new_start = add_state();
  state_type new_accepting = add_state();
  add_transition(new_start, '\x00', start_state);
  add_transition(new_start, '\x00', nfa.start_state + original_size);
  add_transition(*accepting_states.begin(), '\x00', new_accepting);
  add_transition(*nfa.accepting_states.begin() + original_size, '\x00', new_accepting);
  start_state = new_start;

  accepting_states.erase(accepting_states.begin());
  accepting_states.insert(new_accepting);
}

void NFA::augment_table(const NFA& nfa) {
  /*We add all the states of the provided nfa to this one
  * The added states will be calculated as original_size + original_state*/
  state_type original_size = size();

  /*For each entry in the provided NFA table*/
  for(vector<map<char, state_collection_type > >::const_iterator iter = nfa.table.begin(); iter != nfa.table.end(); ++iter) {
    /*We add a state to this table*/
    state_type state = add_state();

    /*And create a mapping for that state. Note that this added state is has index original_size plus the state in nfa.table*/
    map<char, state_collection_type >& transitions = table[state];

    /*We fill the map with the proper keys*/
    for(map<char, state_collection_type >::const_iterator i = iter->begin(); i != iter->end(); ++i) {
      state_collection_type states;
      for(state_collection_type::const_iterator j = (i->second).begin(); j != (i->second).end(); ++j) {
        states.insert(*j + original_size);
      }
      transitions[i->first] = states;
    }
  }
}

ostream& operator<<(ostream& out, const NFA& nfa) {
  out << "Start State: " << nfa.start_state << "\n";
  out << "Accepting States: {";
  for(state_collection_type::const_iterator iter = nfa.accepting_states.begin(); iter != nfa.accepting_states.end(); ++iter) {
    out << *iter << ", ";
  }
  out << "}\n";

  vector<map<char, state_collection_type > >::const_iterator begin = nfa.table.begin();
  vector<map<char, state_collection_type > >::const_iterator end = nfa.table.end();
  for(vector<map<char, state_collection_type > >::const_iterator entry = begin; entry != end; ++entry) {
    out << (entry - begin) << ":\n";
    for(map<char, state_collection_type >::const_iterator transitions = entry->begin(); transitions != entry->end(); ++transitions) {
      if(transitions->first == '\x00')
        out << "\t" << "epsilon" << ": {";
      else
        out << "\t" << transitions->first << ": {";
      bool first = true;
      for(state_collection_type::const_iterator iter = transitions->second.begin(); iter != transitions->second.end(); ++iter) {
        if(!first)
          out << ", " << *iter;
        else {
          out << *iter;
          first = false;
        }
      }
      out << "}\n";
    }
    out << "\n";
  }
  return out;
}


/*Parses the regular expression in the stringstream altering the provided NFA*/
void NFA::parse_expression(stringstream& sstream, NFA& nfa) {
  parse_term(sstream, nfa);

  while(char_traits<char>::to_char_type(sstream.peek()) == '|') {
    match(sstream, '|');
    NFA nfa2; //Create empty nfa and fill it
    parse_term(sstream, nfa2);
    nfa.alternation(nfa2); //Form the alternation of the first nfa with the second
  }
}

/*Parses the term in the stringstream altering the provided NFA*/
void NFA::parse_term(stringstream& sstream, NFA& nfa) {
  parse_factor(sstream, nfa);
  char_traits<char>::int_type in = sstream.peek();
  char test = char_traits<char>::to_char_type(in);

  while(test != ')' && test != '|' && test != '*' && test != '\x00'
     && in != char_traits<char>::eof()) {

    NFA nfa2;
    parse_factor(sstream, nfa2);
    nfa.concatenation(nfa2);

    in = sstream.peek();
    test = char_traits<char>::to_char_type(in);
  }
}

/*Parses the factor in the stringstream altering the provided NFA*/
void NFA::parse_factor(stringstream& sstream, NFA& nfa) {
  parse_base(sstream, nfa);
  if(char_traits<char>::to_char_type(sstream.peek()) == '*') {
    match(sstream, '*');
    nfa.kleene_closure();
  }
  while(char_traits<char>::to_char_type(sstream.peek()) == '*') {
    match(sstream, '*');
  }
}

void NFA::parse_character_class(stringstream& sstream, NFA& nfa) {

  if(char_traits<char>::to_char_type(sstream.peek()) == '^') {
    /*We match only the characters NOT containued in the following class*/
    match(sstream, '^');
    set<char> chars;
    char c;
    while((c = char_traits<char>::to_char_type(sstream.peek())) != ']') {
      match(sstream, c);
      chars.insert(c);
    }
    for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
      /*chars does not contain c*/
      if(chars.find(c) == chars.end() && c != '\x00') {
        if(nfa.size() == 0) {
          match_char(nfa, c);
        } else {
          NFA tmp ;
          match_char(tmp, c);
          nfa.alternation(tmp);
        }
      }
    }
    c = CHAR_MAX;
    if(chars.find(c) == chars.end() && c != '\x00') {
      NFA tmp ;
      match_char(tmp, c);
      nfa.alternation(tmp);
    }
    return;
  }

  parse_character_literal(sstream, nfa);
  while(char_traits<char>::to_char_type(sstream.peek()) != ']') {
    NFA tmp;
    parse_character_literal(sstream, tmp);
    nfa.alternation(tmp);
  }
}

void NFA::parse_character_literal(stringstream& sstream, NFA& nfa) {
  /*If we encounter a backslash, we attempt to escape the next character*/
  if(char_traits<char>::to_char_type(sstream.peek()) == '\\'){
    match(sstream, '\\');
    char_traits<char>::int_type in = sstream.peek();
    char c = char_traits<char>::to_char_type(in);
    if((c != '|' && c != '(' && c != '*' && c != ')' && c != '\\' &&
    c != 'n' && c != 't' && c != '[' && c != ']' && c != '^'
    && c != 'd' && c != 'D' && c != 's' && c != 'S' && c != 'w' && c != 'W' && c != '.' && c != 'a' && c != 'A')
            || in == char_traits<char>::eof()) {
      string msg("Syntax Error: Cannot escape character \'");
      msg += c;
      msg += "\'.";
      throw invalid_argument(msg);
    }
    match(sstream, c);
    if(c == 'n') {
      c = '\n';
    } else if(c == 't') {
      c = '\t';
    } else if (c == 'd') {

      match_digit(nfa);
      return;
    } else if (c == 'D') {
      match_not_digit(nfa);
      return;
    } else if (c == 'w') {
      match_alnum(nfa);
      return;
    } else if (c == 'W') {
      match_not_alnum(nfa);
      return;
    } else if (c == 's') {
      match_whitespace(nfa);
      return;
    } else if (c == 'S') {
      match_not_whitespace(nfa);
      return;
    } else if (c == 'a') {
      match_alpha(nfa);
      return;
    } else if (c == 'A') {
      match_not_alpha(nfa);
      return;
    }
    match_char(nfa, c);
  } else {
    char c = char_traits<char>::to_char_type(sstream.peek());
    match(sstream, c);

    if(c == '.') {
      match_any(nfa);
    } else {
      match_char(nfa, c);
    }
  }
}

void NFA::match_char(NFA& nfa, char c) {
  nfa.start_state = nfa.add_state();
  nfa.accepting_states.insert(nfa.add_state());
  nfa.add_transition(nfa.start_state, c, *(nfa.accepting_states.begin()));
}

void NFA::match_alpha(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && isalpha(c)) {

      if(nfa.size() == 0) {

        match_char(nfa, c);
      } else {
        NFA tmp;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && isalpha(c)) {
    NFA tmp;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }

}

void NFA::match_not_alpha(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && !isalpha(c)) {

      if(nfa.size() == 0) {

        match_char(nfa, c);
      } else {
        NFA tmp;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && !isalpha(c)) {
    NFA tmp;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }

}

void NFA::match_digit(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && isdigit(c)) {

      if(nfa.size() == 0) {

        match_char(nfa, c);
      } else {
        NFA tmp;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && isdigit(c)) {
    NFA tmp;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }

}

void NFA::match_not_digit(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && !isdigit(c)) {
      if(nfa.size() == 0) {
        match_char(nfa, c);
      } else {
        NFA tmp ;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && !isdigit(c)) {
    NFA tmp ;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }
}

void NFA::match_alnum(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && isalnum(c)) {

      if(nfa.size() == 0) {
        match_char(nfa, c);
      } else {
        NFA tmp ;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && isalnum(c)) {
    NFA tmp ;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }
}

void NFA::match_not_alnum(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && !isalnum(c)) {
      if(nfa.size() == 0) {
        match_char(nfa, c);
      } else {
        NFA tmp ;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && !isalnum(c)) {
    NFA tmp ;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }
}

void NFA::match_whitespace(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && isspace(c)) {
      if(nfa.size() == 0) {
        match_char(nfa, c);
      } else {
        NFA tmp ;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && isspace(c)) {
    NFA tmp ;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }
}

void NFA::match_not_whitespace(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00' && !isspace(c)) {
      if(nfa.size() == 0) {
        match_char(nfa, c);
      } else {
        NFA tmp ;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00' && !isspace(c)) {
    NFA tmp ;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }
}

void NFA::match_any(NFA& nfa) {
  char c;
  for(c = CHAR_MIN; c != CHAR_MAX; ++c) {
    /*chars does not contain c*/
    if(c != '\x00') {
      if(nfa.size() == 0) {
        match_char(nfa, c);
      } else {
        NFA tmp ;
        match_char(tmp, c);
        nfa.alternation(tmp);
      }
    }
  }
  c = CHAR_MAX;
  if(c != '\x00') {
    NFA tmp ;
    match_char(tmp, c);
    nfa.alternation(tmp);
  }
}

/*Parses the base in the stringstream altering the provided NFA*/
void NFA::parse_base(stringstream& sstream, NFA& nfa) {
  if(char_traits<char>::to_char_type(sstream.peek()) == '(') {
    match(sstream, '(');
    parse_expression(sstream, nfa);
    match(sstream, ')');
    return;
  }
  if(char_traits<char>::to_char_type(sstream.peek()) == '[') {
    match(sstream, '[');
    parse_character_class(sstream, nfa);
    match(sstream, ']');
    return;
  }
  parse_character_literal(sstream, nfa);
}

/*Consumes the provided character from the stream or throws an exception if the consumed
* character does not match the provided one*/
void NFA::match(stringstream& sstream, const char match) {
  char a;
  sstream.get(a);
  if(a != match) {
    string msg("Syntax Error: Expected character \'");
    msg += match;
    msg += "\' but found \'";
    msg += a;
    msg += "\'.";
    throw invalid_argument(msg);
  }
}
