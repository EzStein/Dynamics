#ifndef DYNSOLVER_REGEX_DFA_H_
#define DYNSOLVER_REGEX_DFA_H_

#include <vector>

#include "regex/common.h"
#include "regex/nfa.h"
#include "regex/regex.h"

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
class dfa : public regex {
 public:
  // Constructs this DFA from the pattern provided. To do so, we first construct
  // an NFA from the pattern. We then use the powerset construction to build
  // a non-optimal DFA. Finally we use Hopcroft's algorithm for DFA minimization
  // to construct a minimal DFA.
  explicit dfa(const std::string& pattern);

  // Returns an integer indicating the length of the smallest
  // matched prefix of candidate if it exists.
  // We ignore characters whose indices are less than startPosition.
  // That is, this function has the effect of
  // finding the longest matched prefix of
  // candidate.substr(startPosition, candidate.size() - startPosition).
  // By default startPosition is 0 indicating that the whole string should
  // be considered. The startPosition value may be anywhere from 0 to
  // candidate.length() inclusive. If it is candidate.length(), then
  // we consider the empty string. If startPosition is out of range then
  // the behavior is undefined (we may assert an error in development mode).
  // 
  // The matched prefix if it exists can be recovered
  // using candidate.substr(startPosition, length) where
  // length is the returned integer. In particular the substring
  // spans from character startPosition (inclusive)
  // to character length (exclusive).
  // Note that if the empty string is the longest matched prefix,
  // this function returns 0. If the pattern does not match any prefix
  // (not even the empty string) then the value -1 is returned.
  int accept_longest_prefix(const std::string& candidate,
                            int startPosition = 0) const override;
  
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
