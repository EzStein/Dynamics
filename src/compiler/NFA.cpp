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

NFA::NFA(const vector<map<char, set<unsigned int> > > & table) {
  this->table = table;
}

bool NFA::accepts(const char * string) {
  set<unsigned int> set = extended_transition_function(start_state, string);
  return set.find(accepting_state) != set.end();
}

set<unsigned int> NFA::extended_transition_function(unsigned int state, const char * string) {
  set<unsigned int> initial;
  set<unsigned int> reachable;
  initial.insert(state);

  /*Iterate over each character*/
  for(const char * current_char = string; *current_char != '\x00'; ++current_char) {

    /*Expand the initial set to include equivalent states over epsilon transitions*/
    for(set<unsigned int>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
      reachable.insert(*iter);
      /*Add epsilon transitions*/
      set<unsigned int> tmp = transition_epsilon(*iter);
      copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
    }
    initial = reachable;
    reachable.clear();

    /*For each state in initial, we add to reachable the reachable states through the given character*/
    for(set<unsigned int>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
      set<unsigned int> tmp = transition_function(*iter, *current_char);
      copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
    }

    /*The reachable sets become the initial ones, and we reset the reachable set to be empty*/
    initial = reachable;
    reachable.clear();
  }

  /*Expand one last time over the initial set to include equivalent states over epsilon transitions*/
  for(set<unsigned int>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
    reachable.insert(*iter);
    /*Add epsilon transitions*/
    set<unsigned int> tmp = transition_epsilon(*iter);
    copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
  }
  initial = reachable;
  reachable.clear();
  return initial;
}

set<unsigned int> NFA::transition_epsilon(unsigned int state) {
  /*Holds a set of states that have been visited by the recursion
  * This is used to exit the recursion if we encounter an infinite loop*/
  static set<unsigned int> visited;

  /*Used to reset the visited flag*/
  static int recursion_level = 0;

  /*True if visited contains state*/
  if(visited.find(state) != visited.end()) {
    return set<unsigned int>(); //An empty set
  }
  visited.insert(state);

  ++recursion_level;

  /*The set of initial states which are the ones immediatly reachable through epsilon transitions*/
  set<unsigned int> initial(transition_function(state, '\x00'));
  set<unsigned int> reachable;
  for(set<unsigned int>::const_iterator iter = initial.begin(); iter != initial.end(); ++iter) {
    set<unsigned int> tmp = transition_epsilon(*iter); //recursive call!!!
    copy(tmp.begin(), tmp.end(), inserter(reachable, reachable.begin()));
  }
  copy(initial.begin(), initial.end(), inserter(reachable, reachable.begin()));
  if(--recursion_level == 0) {
    visited.clear();
  }
  return reachable;
}

const set<unsigned int>& NFA::transition_function(unsigned int state, char transition) {
  return table[state][transition];
}

unsigned int NFA::size() const {
  return table.size();
}

unsigned int NFA::add_state() {
  map<char, set<unsigned int> > empty;
  table.push_back(empty);
  return size() - 1;
}

void NFA::add_transition(unsigned int from_state, char transition_char, unsigned int to_state) {
  table[from_state][transition_char].insert(to_state);
}

void NFA::kleene_closure() {
  /*Add epsilon transition from accepting to start state*/
  add_transition(accepting_state, '\x00', start_state);
  unsigned int new_start = add_state();
  unsigned int new_accepting = add_state();

  /*Add the appropriate epsilon transitions*/
  add_transition(new_start, '\x00', start_state);
  add_transition(accepting_state, '\x00', new_accepting);
  add_transition(new_start, '\x00', new_accepting);
  start_state = new_start;
  accepting_state = new_accepting;
}

void NFA::concatenation(const NFA& nfa) {
  unsigned int original_size = size();
  augment_table(nfa);
  add_transition(accepting_state, '\x00', nfa.start_state + original_size);
  accepting_state = nfa.accepting_state + original_size;
}

void NFA::alternation(const NFA& nfa) {
  unsigned int original_size = size();
  augment_table(nfa);
  unsigned int new_start = add_state();
  unsigned int new_accepting = add_state();
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
  unsigned int original_size = size();

  /*For each entry in the provided NFA table*/
  for(vector<map<char, set<unsigned int> > >::const_iterator iter = nfa.table.begin(); iter != nfa.table.end(); ++iter) {
    /*We add a state to this table*/
    unsigned int state = add_state();

    /*And create a mapping for that state. Note that this added state is has index original_size plus the state in nfa.table*/
    map<char, set<unsigned int> >& transitions = table[state];

    /*We fill the map with the proper keys*/
    for(map<char, set<unsigned int> >::const_iterator i = iter->begin(); i != iter->end(); ++i) {
      set<unsigned int> states;
      for(set<unsigned int>::const_iterator j = (i->second).begin(); j != (i->second).end(); ++j) {
        states.insert(*j + original_size);
      }
      transitions[i->first] = states;
    }
  }
}

ostream& operator<<(ostream& out, const NFA& nfa) {
  out << "Start State: " << nfa.start_state << "\n";
  out << "Accepting State: " << nfa.accepting_state << "\n";
  vector<map<char, set<unsigned int> > >::const_iterator begin = nfa.table.begin();
  vector<map<char, set<unsigned int> > >::const_iterator end = nfa.table.end();
  for(vector<map<char, set<unsigned int> > >::const_iterator entry = begin; entry != end; ++entry) {
    out << (entry - begin) << ":\n";
    for(map<char, set<unsigned int> >::const_iterator transitions = entry->begin(); transitions != entry->end(); ++transitions) {
      if(transitions->first == '\x00')
        out << "\t" << "epsilon" << ": {";
      else
        out << "\t" << transitions->first << ": {";
      bool first = true;
      for(set<unsigned int>::const_iterator iter = transitions->second.begin(); iter != transitions->second.end(); ++iter) {
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
  while(sstream.peek() == '|') {
    match(sstream, '|');
    NFA nfa2; //Create empty nfa and fill it
    parse_term(sstream, nfa2);
    nfa.alternation(nfa2); //Form the alternation of the first nfa with the second
  }
}

/*Parses the term in the stringstream altering the provided NFA*/
void NFA::parse_term(stringstream& sstream, NFA& nfa) {
  parse_factor(sstream, nfa);
  char test = sstream.peek();
  while(test != ')' && test != '|' && test != '*' && test != '\x00' && test != char_traits<char>::eof()) {

    NFA nfa2;
    parse_factor(sstream, nfa2);
    nfa.concatenation(nfa2);
    test = sstream.peek();
  }
}

/*Parses the factor in the stringstream altering the provided NFA*/
void NFA::parse_factor(stringstream& sstream, NFA& nfa) {
  parse_base(sstream, nfa);
  if(sstream.peek() == '*') {
    match(sstream, '*');
    nfa.kleene_closure();
  }
  while(sstream.peek() == '*') {
    match(sstream, '*');
  }
}

/*Parses the base in the stringstream altering the provided NFA*/
void NFA::parse_base(stringstream& sstream, NFA& nfa) {
  if(sstream.peek() == '(') {
    match(sstream, '(');
    parse_expression(sstream, nfa);
    match(sstream, ')');
  } else {
    //We parse a singal character
    char to_add;
    if(sstream.peek() == '\\'){
      match(sstream, '\\');
      char c = sstream.peek();
      if(c != '|' && c != '(' && c != '*' && c != ')' && c != '\\' && c != 'n' && c != 't') {
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
      to_add = sstream.peek();
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
