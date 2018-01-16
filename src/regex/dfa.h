#ifndef DYNSOLVER_REGEX_DFA_H_
#define DYNSOLVER_REGEX_DFA_H_

#include <vector>

#include "regex/common.h"
#include "regex/nfa.h"

namespace dynsolver {
namespace regex {
// The DFA class is one implementation of regular expressions using
// deterministic finite automata. Whereas the NFA implementation generally
// requires less space then the DFA implementation, simulating an NFA is far
// slower than simulating a DFA. If you want a fast regex evaluation at the
// expensive of a larger memory cost, use a DFA. Since this class is an
// implementation detail, all its members are private. regex and lexer
// are friend classes.
//
// This class is optimized for speed. It is implemented using a std::vector
// whose elements are states. Each state contains a fixed kMaxCharacterValue+1
// element array. That is it can hold items indexed from 0 to kMaxCharacterValue
// inclusive. It is an array
// of ints representing the transition from that state on each character.
// Each state also contains a bool value indicating whether it is an accepting
// state or not. DFA's do not have epsilon transitions.
//
// Class invariant: All DFA objects are minimal. That is, upon construction,
// this class will attempt to minimize itself.
class dfa {
 public:
  // Constructs this DFA from the pattern provided. To do so, we first construct
  // an NFA from the pattern. We then use the powerset construction to build
  // a non-optimal DFA. Finally we use Hopcroft's algorithm for DFA minimization
  // to construct a minimal DFA.
  explicit dfa(const std::string& pattern);

  // Returns an integer indicating the length of the matched prefix.
  // The prefix can be recovered using candidate.substr(0, len) where
  // len is the returned integer. In particular the substring
  // spans from character 0 (inclusive) to character len (exclusive).
  // Note that if the empty string is the longest matched prefix,
  // this function returns 0. If the pattern does not match any prefix
  // (not even the empty string) then the value -1 is returned.
  int accept_longest_prefix(const std::string& candidate) const;

  // Returns true if the DFA accepts the candidate string provided
  bool accepts(const std::string& candidate) const;
  
 private:
  // This struct represents on DFA state. The transition array associates to
  // each character (an index from 0 to kMaxCharacterValue inclusive)
  // the state that it transitions to.
  // That integer is itself an index in the vector containing all states. If
  // an element in the array is -1, then there is no transition over that
  // character (equivalently there is a transition to a non-accepting state
  // with only self-loops). A
  // bool is used to indicate if the state is accepting or not.
  struct state {
    int transitions[kNumberOfCharacters];
    bool accepting;

    // The state can be initialized with no arguments.
    state() { }

    // The state can be initialized with the accepting value.
    state(bool _accepting) : accepting(_accepting) { }
  };

  // The underlying data structure of the DFA. The indexes of the vector
  // are used to name each state.
  std::vector<state> graph;

  // There is one start state for the DFA
  int startState;
};
} // namespace regex
} // namespace dynsolver
#endif
