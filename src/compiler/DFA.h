#ifndef DFA_GUARD
#define DFA_GUARD
#include <vector>
#include <map>
#include <set>
#include "NFA.h"


/*
* This class represents a deterministic finite state automata.
* The DFA can be constructed from an NFA or a regular expression.
* Operations include applying the transition function from a state on
* a character and determining whether a string is accepted.
*/
class DFA {
public:
  DFA() { };

  /*Constructs a DFA from a c-style string representing a regular expression*/
  DFA(const char *);
  DFA(const NFA&);

  /*Returns the number of states in this DFA*/
  unsigned int size() const;

  /*Returns true if this DFA accepts the string, and false otherwise*/
  bool accepts(const char *);

private:
  std::vector<std::map<char, unsigned int> > table;
  unsigned int start_state;

  /*The default state is the state that all input strings end up in
  * if a transition rule for a given character is not found. All transitions
  * from the default state return to the default state, and it is not an accepting state*/
  unsigned int default_state;
  std::set<unsigned int> accepting_states;

  /*
  * Returns the state that the DFA would be in on the given string input.
  */
  unsigned int extended_transition_function(unsigned int, const char *);

  /*Adds a state with no transitions to or from it, returns the state number*/
  unsigned int add_state();

  /*Adds a transition from from_state to to_state over the given character*/
  void set_transition(unsigned int from_state, char transition_char, unsigned int to_state);

  /*
  * Uses the subset technique to form a proper DFA from the given NFA.
  */
  static void convert_to_DFA(const NFA&, DFA&);

};

#endif
