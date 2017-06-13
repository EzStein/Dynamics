#include "NFA.h"
#include <iterator>
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <string>
#include <sstream>

using std::set;
using std::map;
using std::vector;
using std::ostream;
using std::endl;
using std::stringstream;
using std::char_traits;
using std::string;
using std::invalid_argument;

NFA::NFA(const char * string) {
  stringstream sstream(string);
  parse_expression(sstream, *this);
}

NFA::NFA(const vector<map<char, set<state_type> > > & table) {
  this->table = table;
}

bool NFA::accepts(const char * string) {
  set<state_type> set = extended_transition_function(start_state, string);
  return set.find(accepting_state) != set.end();
}

set<state_type> NFA::extended_transition_function(state_type state, const char * string) {
  set<state_type> initial;
  set<state_type> reachable;
  initial.insert(state);

  /*Iterate over each character*/
  for(const char * current_char = string; *current_char != '\x00'; ++current_char) {

    /*Expand the initial set to include equivalent states over epsilon transitions*/
    for(set<state_type>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
      reachable.insert(*iter);
      /*Add epsilon transitions*/
      set<state_type> tmp = transition_epsilon(*iter);
      copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
    }
    initial = reachable;
    reachable.clear();

    /*For each state in initial, we add to reachable the reachable states through the given character*/
    for(set<state_type>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
      set<state_type> tmp = transition_function(*iter, *current_char);
      copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
    }

    /*The reachable sets become the initial ones, and we reset the reachable set to be empty*/
    initial = reachable;
    reachable.clear();
  }

  /*Expand one last time over the initial set to include equivalent states over epsilon transitions*/
  for(set<state_type>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
    reachable.insert(*iter);
    /*Add epsilon transitions*/
    set<state_type> tmp = transition_epsilon(*iter);
    copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
  }
  initial = reachable;
  reachable.clear();
  return initial;
}

set<state_type> NFA::transition_epsilon(state_type state) {
  /*Holds a set of states that have been visited by the recursion
  * This is used to exit the recursion if we encounter an infinite loop*/
  static set<state_type> visited;

  /*Used to reset the visited flag*/
  static int recursion_level = 0;

  /*True if visited contains state*/
  if(visited.find(state) != visited.end()) {
    return set<state_type>(); //An empty set
  }
  visited.insert(state);

  ++recursion_level;

  /*The set of initial states which are the ones immediatly reachable through epsilon transitions*/
  set<state_type> initial(transition_function(state, '\x00'));
  set<state_type> reachable;
  for(set<state_type>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
    set<state_type> tmp = transition_epsilon(*iter); //recursive call!!!
    copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
  }
  copy(initial.begin(), initial.end(), inserter(reachable, reachable.begin()));
  if(--recursion_level == 0) {
    visited.clear();
  }
  return reachable;
}

const set<state_type>& NFA::transition_function(state_type state, char transition) {
  return table[state][transition];
}

state_type NFA::size() const {
  return table.size();
}

state_type NFA::add_state() {
  map<char, set<state_type> > empty;
  table.push_back(empty);
  return size() - 1;
}

void NFA::add_transition(state_type from_state, char transition_char, state_type to_state) {
  table[from_state][transition_char].insert(to_state);
}

void NFA::kleene_closure() {
  /*Add epsilon transition from accepting to start state*/
  add_transition(accepting_state, '\x00', start_state);
  state_type new_start = add_state();
  state_type new_accepting = add_state();

  /*Add the appropriate epsilon transitions*/
  add_transition(new_start, '\x00', start_state);
  add_transition(accepting_state, '\x00', new_accepting);
  add_transition(new_start, '\x00', new_accepting);
  start_state = new_start;
  accepting_state = new_accepting;
}

void NFA::concatenation(const NFA& nfa) {
  state_type original_size = size();
  augment_table(nfa);
  add_transition(accepting_state, '\x00', nfa.start_state + original_size);
  accepting_state = nfa.accepting_state + original_size;
}

void NFA::alternation(const NFA& nfa) {
  state_type original_size = size();
  augment_table(nfa);
  state_type new_start = add_state();
  state_type new_accepting = add_state();
  add_transition(new_start, '\x00', start_state);
  add_transition(new_start, '\x00', nfa.start_state + original_size);
  add_transition(accepting_state, '\x00', new_accepting);
  add_transition(nfa.accepting_state + original_size, '\x00', new_accepting);
  start_state = new_start;
  accepting_state = new_accepting;
}

void NFA::augment_table(const NFA& nfa) {
  /*We add all the states of the provided nfa to this one
  * The added states will be calculated as original_size + original_state*/
  state_type original_size = size();

  /*For each entry in the provided NFA table*/
  for(vector<map<char, set<state_type> > >::const_iterator iter = nfa.table.begin(); iter != nfa.table.end(); ++iter) {
    /*We add a state to this table*/
    state_type state = add_state();

    /*And create a mapping for that state. Note that this added state is has index original_size plus the state in nfa.table*/
    map<char, set<state_type> >& transitions = table[state];

    /*We fill the map with the proper keys*/
    for(map<char, set<state_type> >::const_iterator i = iter->begin(); i != iter->end(); ++i) {
      set<state_type> states;
      for(set<state_type>::const_iterator j = (i->second).begin(); j != (i->second).end(); ++j) {
        states.insert(*j + original_size);
      }
      transitions[i->first] = states;
    }
  }
}

ostream& operator<<(ostream& out, const NFA& nfa) {
  out << "Start State: " << nfa.start_state << "\n";
  out << "Accepting State: " << nfa.accepting_state << "\n";
  vector<map<char, set<state_type> > >::const_iterator begin = nfa.table.begin();
  vector<map<char, set<state_type> > >::const_iterator end = nfa.table.end();
  for(vector<map<char, set<state_type> > >::const_iterator entry = begin; entry != end; ++entry) {
    out << (entry - begin) << ":\n";
    for(map<char, set<state_type> >::const_iterator transitions = entry->begin(); transitions != entry->end(); ++transitions) {
      if(transitions->first == '\x00')
        out << "\t" << "epsilon" << ": {";
      else
        out << "\t" << transitions->first << ": {";
      bool first = true;
      for(set<state_type>::const_iterator iter = transitions->second.begin(); iter != transitions->second.end(); ++iter) {
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

/*Parses the base in the stringstream altering the provided NFA*/
void NFA::parse_base(stringstream& sstream, NFA& nfa) {
  if(char_traits<char>::to_char_type(sstream.peek()) == '(') {
    match(sstream, '(');
    parse_expression(sstream, nfa);
    match(sstream, ')');
  } else {
    //We parse a singal character
    char to_add;
    if(char_traits<char>::to_char_type(sstream.peek()) == '\\'){
      match(sstream, '\\');
      char_traits<char>::int_type in = sstream.peek();
      char c = char_traits<char>::to_char_type(in);
      if((c != '|' && c != '(' && c != '*' && c != ')' && c != '\\' && c != 'n' && c != 't')
              || in == char_traits<char>::eof()) {
        string msg("Syntax Error: Cannot escape character \'");
        msg += c;
        msg += "\'.";
        throw invalid_argument(msg);
      }
      if(c == 'n') {
        to_add = '\n';
      } else if(c == 't') {
        to_add = '\t';
      } else {
        to_add = c;
      }
      match(sstream, c);
    } else {

      to_add = char_traits<char>::to_char_type(sstream.peek());
      match(sstream, to_add);
    }
    nfa.start_state = nfa.add_state();
    nfa.accepting_state = nfa.add_state();
    nfa.add_transition(nfa.start_state, to_add, nfa.accepting_state);
  }
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
