#ifndef DYNSOLVER_REGEX_NFA_H_
#define DYNSOLVER_REGEX_NFA_H_

#include <set>
#include <string>
#include <vector>

namespace dynsolver {
namespace regex {

// The nfa class is one implementation of a regular expression using
// non-deterministic finite automata. These NFA's will be constructed using
// Thompson's algorithm. In particular, each state will have at most one
// outward transition per character, although it may have as many as two
// outward epsilon transitions. Each state is assigned a number which is
// its index in the vector that contains it.
//
// Note that this implementation does not admit for a completely general NFA,
// but it will be sufficient for our purposes. The implementation greatly
// simplifies nearly all nfa operations that we will be performing. In
// particular lexer construction will be more complicated since it
// usually requires an arbitrary number of epsilon transitions from
// the start state. This can be circumvented by building a "tree" of
// states connected with epsilon transitions.
class nfa {
 public:
  // These arrays are used to build the special character class NFA's.
  // See the documentation of the regex pattern in regex.h for the exact
  // characters contained in these character classes.
  static const std::vector<char> digitList, nondigitList, whitespaceList,
    nonwhitespaceList, wordList, nonwordList, alphabetList, nonalphabetList,
    anyList;
  
  // This struct represents one NFA state. A state can have one or two out
  // transitions. If transition is a value from 0 to
  // kMaxCharacterValue (inclusive), then the state
  // hase one outward transition. In this case, the transition occurs on the
  // character represented by transition, and the state that it transitions to
  // is stored in outTransition1. If transition value is kDeadStateTransition
  // then there are no outbound transitions (equivalently, all characters
  // transition to a dead state).
  // If transition is a value of kOneEpsilonTransition there is one
  // epsilon transition. The state that it transitions to is stored in
  // out_transition1. If transition is kTwoEpsilonTransition,
  // there are two epsilon transitions
  // stored in outTransition1 and outTransition2. As a convection if we are not
  // using one of the outTransition variables then we set it to -1. This is done
  // purely for debugging purposes (we get some sort of error if we try to access
  // a negative value in the graph vector).
  struct state {
    int transition;
    int outTransition1;
    int outTransition2;
  };

  // These constants are used instead of "magic numbers" when working with the
  // graph implementation of the NFA.
  static constexpr int kDeadStateTransition{-1};
  static constexpr int kOneEpsilonTransition{-2};
  static constexpr int kTwoEpsilonTransition{-3};
  
  // Constructs an nfa from the provided string.
  // This constructor may throw an exception if the pattern string provided is
  // malformed and does not comply with the grammar given below.
  explicit nfa(const std::string& pattern);
  
  // Returns true if this NFA accepts the candidate string provided.
  bool accepts(const std::string& candidate) const;

  // Returns an integer indicating the length of the matched prefix.
  // The prefix can be recovered using candidate.substr(0, len) where
  // len is the returned integer. In particular the substring
  // spans from character 0 (inclusive) to character len (exclusive).
  // Note that if the empty string is the longest matched prefix,
  // this function returns 0. If the pattern does not match any prefix
  // (not even the empty string) then the value -1 is returned.
  int accept_longest_prefix(const std::string& candidate) const;

 private:
  // This is the underlying data structure of the nfa. Each state is an element
  // of a vector. The fields outTransition1 and outTransition2 of the state
  // struct refer to indices in this vector.
  //
  // Class invariant: The graph should not have any cycles of epsilon
  // transitions. The implementation of computing the epsilon closure
  // relies on this fact. In general, Thompson's construction method
  // will not generate any cycles of epsilon transitions.
  std::vector<state> graph;

  // Each nfa must have a start state. That is this value.
  int startState;

  // Using Thompson's construction, any NFA will only have exactly one
  // accepting state. In any case, all NFA's can be made to have one
  // accepting state by adding epsilon transitions.
  int acceptingState;

  // Computes the epsilon closure of the collection of sets provided
  // by appending the result to the outputCollection set.
  // The epsilon closure of a collection of sets is the set of states
  // reacheable through epsilon transitions including the original sets
  // themselves.
  void epsilon_closure(const std::set<int>& inputCollection,
                       std::set<int>& outputCollection) const;

  // Computes the set of states reacheable through the given character
  // transition from the input states by appending these states to the output
  // collection. Note that this function will NOT resolve epsilon transitions.
  void transition_function(const std::set<int>& inputCollection,
                           char transitionCharacter,
                           std::set<int>& outputCollection) const;
  friend class dfa;
};
} // namespace regex
} // namespace dynsolver
#endif
