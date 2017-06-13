#ifndef NFA_GUARD
#define NFA_GUARD
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "reg_def.h"

/*
* This class represents a non-deterministic finite state automata.
* The NFA can be constructed from a DFA or a regular expression.
* Operations include applying the transition function from a set of states on
* a character and determining whether a string is accepted.
* States are represented as integers.
* This class is immutable in the sense that none of its PUBLIC member
* functions alter the state of the class.
* Implementation Details:
* The states are numbered sequentially from 0 to size() - 1.
* Two of the states are special. One is the start state, the other is the only accepting state.
* There will be only one accepting state for now.
* We will use the zero (null) character '\x00'
* to represent an epsilon transition.
*/
class NFA {
public:

  NFA() { };

  /*Constructs a NFA from a c-style string representing a regular expression*/
  NFA(const char *);

  /*Returns the number of states in this NFA*/
  state_type size() const;

  /*Returns true if this NFA accepts the string, and false otherwise*/
  bool accepts(const char *);


private:
  state_type start_state;
  state_type accepting_state;

  /*Returns the set of all states reachable through epsilon transitions from the given state.
  * This function uses recursion to find all such values, and will detect an infinite loop
  * and accomodate it. The function is not guarenteed to return a set containing the input state
  * NOTE: This function is currently implemented using recursion, however we can use a graph reachability
  * algorithm to substitute the recursive "stack" for an actual stack*/
  std::set<state_type> transition_epsilon(state_type);

  /*
  * Returns the set of states reachable by the NFA under the c-style string provided.
  * NOTE: epsilon transitions WILL be resolved.
  */
  std::set<state_type> extended_transition_function(state_type, const char *);

  /*Constructs the NFA from the table by performing a deep copy of all of its data*/
  NFA(const std::vector<std::map<char, std::set<state_type> > > &);

  /*This implements the table as a vector of maps. This may not be the most efficient,
  but it is the most convenient. It may be changed later*/
  std::vector<std::map<char, std::set<state_type> > > table;

  /*The following functions wrap the underlying implementation so that it may change later*/
  /*Returns the set of states which may be empty,
  * that are recheable from the given state over the given char transition
  * NOTE: this function will NOT resolve epsilon transitions*/
  const std::set<state_type>& transition_function(state_type, char);

  /*Adds a state with no transitions to or from it, returns the state number*/
  state_type add_state();

  /*Adds a transition from from_state to to_state over the given character*/
  void add_transition(state_type from_state, char transition_char, state_type to_state);

  /* For these operations to function, the input NFA and the current NFA must have
  * at least one state. No checking is performed.
  */
  /*Changes this NFA to represent the concatentation of itself with the provided NFA*/
  void concatenation(const NFA&);

  /*Changes this NFA to represent the alternation of itself with the provided NFA*/
  void alternation(const NFA&);

  /*Changes this NFA to represent the kleene closure of itself*/
  void kleene_closure();

  /*augments this NFA's adjacency table with that of the given NFA.
  * This function adds new states to this NFA and constructs the given NFA into those states
  * NOTE: the new index of the states in the provided nfa is calculated by adding the original_size
  * of this nfa to the index of the state in in the given NFA*/
  void augment_table(const NFA&);


  /*Parses the regular expression in the stringstream altering the provided NFA*/
  static void parse_expression(std::stringstream&, NFA&);

  /*Parses the term in the stringstream altering the provided NFA*/
  static void parse_term(std::stringstream&, NFA&);

  /*Parses the factor in the stringstream altering the provided NFA*/
  static void parse_factor(std::stringstream&, NFA&);

  /*Parses the base in the stringstream altering the provided NFA*/
  static void parse_base(std::stringstream&, NFA&);

  /*Consumes the provided character from the stream or throws an exception if the consumed
  * character does not match the provided one*/
  static void match(std::stringstream&, const char);


  friend std::ostream& operator<<(std::ostream& out, const NFA& nfa);
  friend int main();

  friend class DFA;
};

//std::ostream& operator<<(std::ostream& out, const NFA& nfa);

#endif
