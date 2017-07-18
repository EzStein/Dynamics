#ifndef NFA_GUARD
#define NFA_GUARD
#include <vector>
#include <map>
#include <iostream>
#include "compiler/regex/reg_def.h"

/*
* This class represents a non-deterministic finite state automaton.
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

  NFA(const std::string& str);

  /*Constructs a NFA from a c-style string representing a regular expression*/
  NFA(const char *);

  /*Returns the number of states in this NFA*/
  state_type size() const;

  /*Returns true if this NFA accepts the string, and false otherwise*/
  bool accepts(const char *) const;

  /*This function simulates the NFA until termination, which occurs if the input stream
  * runs out of characters, or the NFA reaches a nonrecoverable empty state. The function returns the
  * longest prefix of the input state that is accepted by the NFA as well as the set of
  * accepting states that the NFA could be in after matching that prefix. If the nfa matches the empty string,
  * it may return that string. If no prefix (not even the empty string) is matched by the nfa,
  * the string containing the single null character '\x00' is returned along with an empty set of states.
  * The function consumes the prefix in the input stream leaving it to point to the next available
  * character after the prefix.
  *
  * If the third argument is set to true, the NFA will discard whitespace as it reads.
  */
  state_collection_type accept_longest_prefix(std::istream&, std::string&, bool ignoreWhitespace) const;

  /*
  * Returns a reference to the set of accepting states in this NFA.
  */
  const state_collection_type& get_accepting_states() const;

private:
  state_type start_state;
  state_collection_type accepting_states;

  /*Returns the set of all states reachable through epsilon transitions from the given state including this state.
  * This function uses a stack and a graph reachability algorithm to find all such values.*/
  state_collection_type epsilon_closure(state_type) const;

  /*
  * The same as the previous function except that it computes the union of the
  * epsilon closures over all elements of its input
  */
  state_collection_type epsilon_closure(state_collection_type) const;

  /*
  * Returns the set of states reachable by the NFA under the c-style string provided.
  * NOTE: epsilon transitions WILL be resolved.
  */
  state_collection_type extended_transition_function(state_type, const char *) const;

  /*Constructs the NFA from the table by performing a deep copy of all of its data*/
  NFA(const std::vector<std::map<char, state_collection_type > > &);

  /*This implements the table as a vector of maps. This may not be the most efficient,
  but it is the most convenient. It may be changed later.
  Each entry in the vector represents a state, whose out transitions for a given character
  are the states in the set mapped to that character*/
  std::vector<std::map<char, state_collection_type > > table;

  /*The following functions wrap the underlying implementation so that it may change later*/
  /*Returns the set of states which may be empty,
  * that are recheable from the given state over the given char transition
  * NOTE: this function will NOT resolve epsilon transitions*/
  state_collection_type transition_function(state_type, char) const;

  /*
  * Performs the same operation as the previous function except it calculates the union of the
  * transition function applied to all states in the provided collection.
  */
  state_collection_type transition_function(state_collection_type states, char transition) const;

  /*Adds a state with no transitions to or from it, returns the state number*/
  state_type add_state();

  /*Adds a transition from from_state to to_state over the given character*/
  void add_transition(state_type from_state, char transition_char, state_type to_state);

  /* For these operations to work correctly, the input NFA and the current NFA must have
  * at least one state. No checking is performed. Additionally, both the NFAs must have
  * exactly one accepting state in accepting_states, namely *accepting_states.begin()
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
  * of this nfa to the index of the state in in the given NFA
  * NOTE: this function does NOT alter accepting_states or start_state*/
  void augment_table(const NFA&);


  /*Parses the regular expression in the stringstream altering the provided NFA*/
  /* It will parse the following grammer using top-down recursive descent. GRAMMER:
  *    E -> T '|' E
  *    T -> F T
  *    F -> B
  *    F -> F *
  *    B -> character
  *    B -> ( E )
  *    character -> \n | \t | \\ | \'|' | \* | \( | \) | literal
  *    literal -> a | b | c ... | A | B | C | ... | 1 | 2 | 3 | ... | ! | @ | # | ...
  */
  static void parse_expression(std::stringstream&, NFA&);

  /*Parses the term in the stringstream altering the provided NFA*/
  static void parse_term(std::stringstream&, NFA&);

  /*Parses the factor in the stringstream altering the provided NFA*/
  static void parse_factor(std::stringstream&, NFA&);

  static void parse_character_literal(std::stringstream& sstream, NFA& nfa);

  /*Parses the base in the stringstream altering the provided NFA*/
  static void parse_base(std::stringstream&, NFA&);

  /*Consumes the provided character from the stream or throws an exception if the consumed
  * character does not match the provided one*/
  static void match(std::stringstream&, const char);

  static void parse_character_class(std::stringstream& sstream, NFA& nfa);
  static void match_any(NFA& nfa);
  static void match_char(NFA& nfa, char);
  static void match_digit(NFA& nfa);
  static void match_not_digit(NFA& nfa);
  static void match_alnum(NFA& nfa);
  static void match_not_alnum(NFA& nfa);
  static void match_whitespace(NFA& nfa);
  static void match_not_whitespace(NFA& nfa);
  static void match_alpha(NFA& nfa);
  static void match_not_alpha(NFA& nfa);


  friend std::ostream& operator<<(std::ostream& out, const NFA& nfa);
  friend int main();

  friend class DFA;
  friend class lexer;
};

#endif
