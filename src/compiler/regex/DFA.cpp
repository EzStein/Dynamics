#include <stack>
#include <set>
#include "compiler/DFA.h"

using std::map;
using std::vector;
using std::stack;
using std::set;

DFA::DFA(const char * string) {
  NFA nfa(string);
  convert_to_DFA(nfa, *this);
}

DFA::DFA(const NFA& nfa) {
  convert_to_DFA(nfa, *this);
}

void DFA::convert_to_DFA(const NFA& nfa, DFA& dfa) {
  /*Create a start state*/
  dfa.default_state = dfa.add_state();
  dfa.start_state = dfa.add_state();
  stack<state_collection_type> processingStack;

  /*A mapping from NFA collections of states to dfa states*/
  map<state_collection_type, state_type> nfaToDfaMap;

  /*Initially the stack contains only the epsilon closure of the start state of the nfa*/
  state_collection_type states = nfa.epsilon_closure(nfa.start_state);
  processingStack.push(states);
  nfaToDfaMap[states] = dfa.start_state;
  while(!processingStack.empty()) {
    states = processingStack.top();
    processingStack.pop();

    /*This set holds all the possible transitions out of the current collection of states*/
    set<char> transitions;
    /*Iterate over each state to find possible transitions*/
    /*For each state in states*/
    for(state_collection_type::const_iterator state = states.begin(); state != states.end(); ++state) {
      /*Find all transitions out of that state and add them*/
      for(map<char, state_collection_type>::const_iterator pair = nfa.table[*state].begin(); pair != nfa.table[*state].end(); ++pair) {
        transitions.insert(pair->first);
      }
    }

    /*We now iterate over all possible transitions*/
    for(set<char>::const_iterator transition = transitions.begin(); transition != transitions.end(); ++transition) {
      state_collection_type newStates = nfa.epsilon_closure(nfa.transition_function(states, *transition));
      /*If the map does not contain the given key*/
      if(nfaToDfaMap.find(newStates) == nfaToDfaMap.end()) {
        processingStack.push(newStates);
        state_type id = dfa.add_state();
        nfaToDfaMap[newStates] = id;

        //If the new states contain an accepting state we mark the constructed state as accepting.
        if(intersects(newStates.begin(), newStates.end(), nfa.accepting_states.begin(), nfa.accepting_states.end())) {
          dfa.accepting_states.insert(id);
        }
      }
      dfa.set_transition(nfaToDfaMap[states], *transition, nfaToDfaMap[newStates]);
    }
  }

  /*This is a list of states that need to be processed.
  * In general, it will grow as we iterate through it. Since it is a list,
  * none of its iterators will be invalidated when we insert items into it.
  * Any elements before the element currently being examined by the iterators
  * are considered marked and already processed*/
}

bool DFA::accepts(const char * string) const {
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

state_type DFA::extended_transition_function(state_type state, const char * current_char) const {
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
