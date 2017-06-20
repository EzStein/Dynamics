#include "DFA.h"
#include <stack>

using std::map;
using std::vector;
using std::stack;

DFA::DFA(const char * string) {
  NFA nfa(string);
  convert_to_DFA(nfa, *this);
}

DFA::DFA(const NFA& nfa) {
  convert_to_DFA(nfa, *this);
}

void DFA::convert_to_DFA(const NFA& nfa, DFA& dfa) {
  /*Create a start state*/
  dfa.start_state = dfa.add_state();
  dfa.default_state = dfa.add_state();
  state_type state = nfa.start_state;

  stack<state_collection_type> processingStack;
  //processingStack.push(nfa.epsilon_closure);

  /*This is a list of states that need to be processed.
  * In general, it will grow as we iterate through it. Since it is a list,
  * none of its iterators will be invalidated when we insert items into it.
  * Any elements before the element currently being examined by the iterators
  * are considered marked and already processed*/

}

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

    /*Get the transition map for the current state*/
    map<char, state_type> transitionsMap = table[state];
    map<char, state_type>::const_iterator elem = transitionsMap.find(*current_char);

    /*True if the map contains the entry*/
    if(elem != transitionsMap.end()) {
      state = elem->second;
    } else {
      return default_state;
    }
  }
  return state;
}
