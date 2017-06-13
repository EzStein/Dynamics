#include "DFA.h"
#include <list>

using std::list;
using std::map;
using std::vector;

DFA::DFA(const char * string) {
  NFA nfa(string);
  //convert_to_DFA(nfa, *this);
}

DFA::DFA(const NFA& nfa) {
  //convert_to_DFA(nfa, *this);
}

//void DFA::convert_to_DFA(const NFA& nfa, DFA& dfa) {
  /*Create a start state*/
//  dfa.start_state = dfa.add_state();
//  state_type state = nfa.start_state;

  /*This is a list of states that need to be processed.
  * In general, it will grow as we iterate through it. Since it is a list,
  * none of its iterators will be invalidated when we insert items into it.
  * Any elements before the element currently being examined by the iterators
  * are considered marked and already processed*/
  //list<state_type> states_processing;
//}

bool DFA::accepts(const char * string) {
  /*True if accepting states contains the state given by extended_transition_function*/
  return accepting_states.find(extended_transition_function(start_state, string))
          != accepting_states.end();
}

state_type DFA::size() const {
  return table.size();
}

state_type DFA::add_state() {
  map<char, state_type> val;
  table.push_back(val);
  return size() - 1;
}

void DFA::set_transition(state_type from_state, char transition_char, state_type to_state) {
  table[from_state][transition_char] = to_state;
}

state_type DFA::extended_transition_function(state_type state, const char * current_char) {
  /*Iterate over each character in the string*/
  for(; *current_char != '\x00'; ++current_char) {
    map<char, state_type> transitions_map = table[state];
    map<char, state_type>::const_iterator elem = transitions_map.find(*current_char);

    /*True if the map contains the entry*/
    if(elem != transitions_map.end()) {
      state = elem->second;
    } else {
      state = default_state;
    }
  }
  return state;
}
