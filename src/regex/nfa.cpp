#include "regex/nfa.h"

#include <cassert>
#include <cctype>
#include <stack>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "regex/common.h"
#include "regex/malformed_pattern_exception.h"

namespace dynsolver {
namespace regex {
// We forward declare all of our helper functions here so that we can define
// them after the class member definitions.
namespace {
// Since it is obvious that we are working with the nfa::state, we have typedef
// statement.
typedef nfa::state state;

void parse_expression(const std::string& input, int& pointer,
                      std::vector<state>& graph, int* start,
                      int* accepting);
void parse_term(const std::string& input, int& pointer,
                std::vector<state>& graph, int* start,
                int* accepting);
void parse_factor(const std::string& input, int& pointer,
                  std::vector<state>& graph, int* start,
                  int* accepting);
void parse_base(const std::string& input, int& pointer,
                std::vector<state>& graph, int* start,
                int* accepting);
void parse_literal(const std::string& input, int& pointer,
                   std::vector<state>& graph, int* start,
                   int* accepting);
void parse_character_class(const std::string& input, int& pointer,
                           std::vector<state>& graph, int* start,
                           int* accepting);
void match(const std::string& input, int& pointer, char c);
void build_character_nfa(std::vector<state>& graph, int* start,
                         int* accepting, char c);
void build_alternation_nfa(std::vector<state>& graph, int* start,
                           int* accepting, const std::vector<char>& array);
bool is_term_prefix(char c);
bool is_unary_operator_character(char c);
std::vector<char> make_digit_list();
std::vector<char> make_nondigit_list();
std::vector<char> make_whitespace_list();
std::vector<char> make_nonwhitespace_list();
std::vector<char> make_word_list();
std::vector<char> make_nonword_list();
std::vector<char> make_alphabet_list();
std::vector<char> make_nonalphabet_list();
std::vector<char> make_any_list();
}

// We initialize the static const members of this class. This happens exactly
// once, on program start.
const std::vector<char> nfa::digitList(make_digit_list()),
  nfa::nondigitList(make_nondigit_list()),
  nfa::whitespaceList(make_whitespace_list()),
  nfa::nonwhitespaceList(make_nonwhitespace_list()),
  nfa::wordList(make_word_list()),
  nfa::nonwordList(make_nonword_list()),
  nfa::alphabetList(make_alphabet_list()),
  nfa::nonalphabetList(make_nonalphabet_list()),
  nfa::anyList(make_any_list());
  
// Definition of public members.
nfa::nfa(const std::string& pattern) {
  // Generates the nfa graph and stores the result in graph
  // as well as the resulting startState and acceptingState. Pointer is
  // initally set to zero to indicate we are starting at the beginning of the
  // string. We throw an exception if the pointer does not point to the end
  // of the string after the function call.
  int pointer = 0;
  parse_expression(pattern, pointer, graph, &startState, &acceptingState);
  if(pointer != pattern.size()) {
    // Something went wrong with the parse. The string was not parsed completely.
    throw malformed_pattern_exception(
        "Malformed Pattern Exception at position "
        + std::to_string(pointer) +
        ": Parse terminated prematurely.");
  }
}

bool nfa::accepts(const std::string& candidate) const {
  // We simply check if the longest accepted prefix of candidate is
  // candidate itself.
  return accept_longest_prefix(candidate) == candidate.size();
}

int nfa::accept_longest_prefix(const std::string& candidate) const {
  // We find the longest prefix is by simulating the nfa until termination.
  // The nfa terminates once there are no more current states to transition
  // from, or we have reached the end of input. As we go, we keep track
  // of which position in the input was the last to admit an accepting state.
  // This value is initially set to -1. We then check (via resolving epsilon
  // transitions) if this nfa accepts the empty string. If so the value is set
  // to 0. Then we simulate the input on the nfa, setting the value to the new
  // character pointer whenever we reach an accepting state.
  //
  // We use two std::set to store which states we are currently in and which states
  // we will transition to. We swap the two lists on each iteration.
  int pointer(0);
  int longestPrefixSeen(-1);
  std::set<int> currentStates;
  std::set<int> nextStates;

  currentStates.insert(startState);
  epsilon_closure(currentStates, nextStates);
  if(nextStates.find(acceptingState) != nextStates.end()) {
    // The epsilon closure contains an accepting state, so
    // we set longest prefix seen to 0
    longestPrefixSeen = 0;
  }
  std::swap(currentStates, nextStates);
  nextStates.clear();

  // Invariant: the epsilon closure of currentStates must be currentStates
  // itself.
  while(pointer != candidate.size() && !currentStates.empty()) {
    transition_function(currentStates, candidate[pointer], nextStates);
    std::swap(currentStates, nextStates);
    nextStates.clear();
    epsilon_closure(currentStates, nextStates);
    if(nextStates.find(acceptingState) != nextStates.end()) {
      // + 1 since longest prefix seen is half open interval. 
      longestPrefixSeen = pointer + 1;
    }
    std::swap(currentStates, nextStates);
    nextStates.clear();
    ++pointer;
  }
  return longestPrefixSeen;
}

// Definition of private members.
void nfa::epsilon_closure(const std::set<int>& inputCollection,
                          std::set<int>& outputCollection) const {
  // We use a depth first search to find the closure. Since the graph contains
  // no cycles of epsilon transitions, the function will terminate.
  // We first push all the input states onto a stack. While the stack is not
  // empty, we continually pop the stack and add to it all states reacheable
  // from the popped state. We also add the popped state to the outputCollection.
  std::stack<int> stateStack;
  for(std::set<int>::const_iterator iter = inputCollection.begin();
      iter != inputCollection.end(); ++iter) {
    stateStack.push(*iter);
  }

  while(!stateStack.empty()) {
    int stateIndex = stateStack.top();
    stateStack.pop();
    outputCollection.insert(stateIndex);
    state state(graph[stateIndex]);
    if(state.transition == nfa::kOneEpsilonTransition) {
      stateStack.push(state.outTransition1);
    } else if(state.transition == nfa::kTwoEpsilonTransition) {
      stateStack.push(state.outTransition1);
      stateStack.push(state.outTransition2);
    }
  }
}

void nfa::transition_function(const std::set<int>& inputCollection,
                              char c,
                              std::set<int>& outputCollection) const {
  // We first cast transitionCharacter to an unsigned char before promoting it
  // to an int. This ensures that it is consistent with the 0 to
  // kMaxCharacterValue range of state.transition
  int transitionCharacter = static_cast<unsigned char>(c);
  for(std::set<int>::const_iterator iter = inputCollection.begin();
      iter != inputCollection.end(); ++iter) {
    state state(graph[*iter]);
    if(state.transition == transitionCharacter) {
      outputCollection.insert(state.outTransition1);
    }
  }
}

// The following are helper methods used for constructing the nfa from a string.
// They are kept in an anonymous namespace so as not to clutter the header with
// unnecessary implementation details.
namespace {
// The follow functions construct the graph of an nfa from a regular expression
// using recursive descent
// parsing. Streams will not be used since they are esoteric and complicated
// to use correctly while handling errors. Instead, each of the following
// functions will share the input string, and a character pointer which
// indicates the character currently being examined. Additionally,
// the functions share a std::vector<state> object which is filled
// during the parse. If at any point, the string cannot be parsed,
// a malformed_pattern_exception is thrown containing a string
// describing the error. The grammar for the regular expresions
// is show below. Note that the character | will show alternative
// productions from one rule unless it is surrounded by single quotes.
// In that case, it will literally mean the pipe character.
//
// E -> T
// E -> T '|' E
// T -> F
// T -> F T
// F -> B    followed by zero or more instances of a * + ?
// B -> ( E )
// B -> [ C ]
// B -> L
// L -> \( | \) | \[ | \] | \* | \+ | \? | \'|' | \\ | \. |
//      \d | \D | \s | \S | \w | \W | \a | \A | N
// N -> any character except ()[]*+?|\
// C -> 1 or more literals (L)
//
// In the grammar, E stands for expression, T for Term, F for Factor,
// B for base, C for Character Class, L for Literal, N for Normal character,
// and S for String. Each of these nonterminals has a dedicated function
// which parses it. In the following functions, input is the shared input
// string, pointer is a shared integer pointing to a character in the input
// string which is currently being examined, and graph is the nfa graph that
// is being constructed. Additionally, each function needs to return which
// of the states that it constructed is considered the start state and the
// accepting state. This allows the calling function to correctly construct
// its own nfa fragment. The last two arguments are pointers that allow
// for this.

// For all the parse functions we maintain the following invariant.
// When a parse function is called, it adds the states representing the parsed
// expression to the graph vector and then returns via pointer the index of
// the resultant start and accepting states. The added the accepting state
// that is returned MUST have no outbound transitions. We will use an assert
// at the end of each function to verify this during development.
void parse_expression(const std::string& input, int& pointer,
                           std::vector<state>& graph, int* start,
                           int* accepting) {
  // It is possible that two expressions are parsed. If so we build the
  // alternation of both. We call the accepting/start state of the expressions
  // acceptingStateA and acceptingStateB
  int acceptingStateA;
  int startStateA;
  parse_term(input, pointer, graph, &startStateA, &acceptingStateA);
  
  // If the term is followed by a '|' we parse an expression and form the
  // alternation of the two expressions. We also check to see if we have
  // reached the end of the input string. Short circuiting is used to ensure
  // that we do not attempt to access an out of bounds portion of the string.
  if(pointer < input.size() && input[pointer] == '|') {
    ++pointer; // Consume the character
    int startStateB;
    int acceptingStateB;
    parse_expression(input, pointer, graph, &startStateB, &acceptingStateB);

    // We now construct the alternation of the two expressions. To do so, we
    // create a start state and add from it an epsilon transition to each the
    // start states of both nfa fragments. We also add an accepting state
    // and an epsilon transition from the accepting states of the two
    // fragments to the new accepting state.

    // Add start state
    *start = graph.size();
    graph.push_back(state{nfa::kTwoEpsilonTransition, startStateA, startStateB});

    // Add accepting state
    *accepting = graph.size();
    graph.push_back(state{nfa::kDeadStateTransition,-1,-1});
    
    // We now change the accepting state of both fragments to point to the
    // new accepting state using epsilon transitions.
    graph[acceptingStateA] = state{nfa::kOneEpsilonTransition, *accepting, -1};
    graph[acceptingStateB] = state{nfa::kOneEpsilonTransition, *accepting, -1};

    // The function now exits and the parameters start and accepting are
    // already set to the correct values.
  } else {
    // If only one expression is parsed, the starting and accepting states are
    // forwarded to the calling function
    *start = startStateA;
    *accepting = acceptingStateA;
  }
  assert(graph[*accepting].transition == nfa::kDeadStateTransition); // Verify invariant
}

void parse_term(const std::string& input, int& pointer,
                     std::vector<state>& graph, int* start, int* accepting) {
  int startStateA;
  int acceptingStateA;
  parse_factor(input, pointer, graph, &startStateA, &acceptingStateA);
  
  // We now check to see if the next character can be a prefix of a Term.
  // In general, all characters may be a prefix of a term nonterminal
  // except the following: )]*+?^|
  // A helper function is used to determine if this is the case.
  if(pointer < input.size() && is_term_prefix(input[pointer])) {
    // In this case, we parse a term and construct a concatenation
    int startStateB;
    int acceptingStateB;
    parse_term(input, pointer, graph, &startStateB, &acceptingStateB);

    // We do not need to add any more states, however we must add an epsilon
    // transition from acceptingStateA to startStateB.
    graph[acceptingStateA] = state{nfa::kOneEpsilonTransition, startStateB, -1};

    // We now set the appropriate return values.
    *start = startStateA;
    *accepting = acceptingStateB;
  } else {
    // Otherwise we forward the return values of the original factor parsed.
    *start = startStateA;
    *accepting = acceptingStateA;
  }
  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

void parse_factor(const std::string& input, int& pointer,
                       std::vector<state>& graph, int* start, int* accepting) {
  int startState;
  int acceptingState;
  parse_base(input, pointer, graph, &startState, &acceptingState);

  // This loop is used to continually consume * ? and + characters. Each time
  // it runs, it appends the appropriate nfa fragment to the vector. We have
  // the following loop invariant. The variables startState and acceptingState
  // point to the respective states of the newly created nfa fragment upon
  // entering the loop guard.
  while(pointer < input.size() && is_unary_operator_character(input[pointer])) {
    if(input[pointer] == '*') {
      // In the kleene star case we add two states. The new start state
      // transitions to startState and to the new accepting state.
      // We also add a transition from acceptingState to startState
      // and to the new accepting state.
      int newStartState = graph.size();
      int newAcceptingState = newStartState + 1;
      graph.push_back(state{nfa::kTwoEpsilonTransition,
              startState, newAcceptingState}); // Start
      graph.push_back(state{nfa::kDeadStateTransition, -1, -1}); // Accepting
      graph[acceptingState] = state{nfa::kTwoEpsilonTransition,
                                    newAcceptingState, startState};

      // We maintain the invariant with the following two lines
      startState = newStartState;
      acceptingState = newAcceptingState;
    } else if(input[pointer] == '+') {
      // In this case, we add a new accepting state, and set two transitions
      // from acceptingState to newAcceptingState, and startState.
      int newAcceptingState = graph.size();
      graph.push_back(state{nfa::kDeadStateTransition, -1, -1});
      graph[acceptingState] = state{nfa::kTwoEpsilonTransition, newAcceptingState, startState};
      
      acceptingState = newAcceptingState;
    } else if(input[pointer] == '?') {
      // Adds a new start state only. We form a transition from
      // the new start state to startState and to acceptingState.
      int newStartState = graph.size();
      graph.push_back(state{nfa::kTwoEpsilonTransition, startState, acceptingState});
      startState = newStartState;
    }
    ++pointer; // Consume the character
  }

  // We set the returned values appropriately
  *start = startState;
  *accepting = acceptingState;
  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

void parse_base(const std::string& input, int& pointer,
                     std::vector<state>& graph, int* start, int* accepting) {
  // We first check the character. If it is ( or [ we parse an expression
  // and a character class respectively. The start and accepting states
  // are forwarded from the parsed expressions.
  if(pointer < input.size() && input[pointer] == '(') {
    ++pointer; // Consume the '('
    parse_expression(input, pointer, graph, start, accepting);
    match(input, pointer, ')'); // We expect a closed parenthesis
  } else if(pointer < input.size() && input[pointer] == '[') {
    ++pointer; // Consume the '['
    parse_character_class(input, pointer, graph, start, accepting);
    match(input, pointer, ']');
  } else {
    parse_literal(input, pointer, graph, start, accepting);
  }
  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

void parse_literal(const std::string& input, int& pointer,
                        std::vector<state>& graph, int* start, int* accepting) {
  // We first check to see if we have a backslash character. If so, we expect
  // that the next character is an escapable character and parse it accordingly.
  // If that fails, we throw an exception. We also throw an exception if we have
  // reached the end of input unexpectedly.

  if(pointer >= input.size()) {
    // We have reached the end of input prematurely
    throw malformed_pattern_exception("Malformed Pattern Exception at position"
                                      + std::to_string(pointer - 1) +
                                      ": end of input"
                                      " reached prematurely.");
  }
  // We may now assume that we have not reached the end of input
  if(input[pointer] == '\\') {
    // If we have reached a backslash character, we consume it and attempt
    // to consume an escapable character.
    ++pointer;
    // We may have reached the end of input again, so we check it and throw
    // an exception if necessary.
    if(pointer >= input.size()) {
      throw malformed_pattern_exception(
          "Malformed Pattern Exception at position "
          + std::to_string(pointer - 1) +
          ": end of input reached prematurely."
          " Expected escapable character after backslash."
          " The following characters are"
          " escapable: ()[]*+?|\\.dDsSwWaA");
    }

    // We can now assume that we have not reached the end of input
    switch(input[pointer]) {
      case '(':
      case ')':
      case '[':
      case ']':
      case '*':
      case '+':
      case '?':
      case '|':
      case '\\':
      case '.':
        // In all these cases, we construct an nfa fragment that matches the
        // character input[pointer]. We forward the resulting state and
        // accepting state values to the caller
        build_character_nfa(graph, start, accepting, input[pointer]);
        break;
      case 'd':
        build_alternation_nfa(graph, start, accepting, nfa::digitList);
        break;
      case 'D':
        build_alternation_nfa(graph, start, accepting, nfa::nondigitList);
        break;
      case 'w':
        build_alternation_nfa(graph, start, accepting, nfa::wordList);
        break;
      case 'W':
        build_alternation_nfa(graph, start, accepting, nfa::nonwordList);
        break;
      case 's':
        build_alternation_nfa(graph, start, accepting, nfa::whitespaceList);
        break;
      case 'S':
        build_alternation_nfa(graph, start, accepting, nfa::nonwhitespaceList);
        break;
      case 'a':
        build_alternation_nfa(graph, start, accepting, nfa::alphabetList);
        break;
      case 'A':
        build_alternation_nfa(graph, start, accepting, nfa::nonalphabetList);
        break;
      default:
        // In the default case, we have encountered some other character which
        // is not escapable. We throw an exception with a message.
        throw malformed_pattern_exception(
            "Malformed Pattern Exception at position "
            + std::to_string(pointer) +
            ": Expected escapable character after the backslash but found \'"
            + input[pointer] +
            "\'. The following characters are"
            " escapable: ()[]*+?|\\.dDsSwWaA");
    }
    // We finally consume the input and return from the function
    ++pointer;
    assert(graph[*accepting].transition == nfa::kDeadStateTransition);
    return;
  }

  // Here we can assume that we have not reached the end of input
  // and the character pointed to is not a backslash.
  // If the character is a ()[]*+?^| then we throw an error.
  // If the character is a . we parse a character class.
  // Otherwise we build an nfa fragment that matches that character.
  switch(input[pointer]) {
    case '(':
    case ')':
    case '[':
    case ']':
    case '*':
    case '+':
    case '?':
    case '|':
      throw malformed_pattern_exception(
          "Malformed Pattern Exception at position "
          + std::to_string(pointer) +
          ": Expected non-control character but found '"
          + input[pointer] +
          "'. Non control characters are any character except "
          "()[]*+?|\\");
    case '.':
      build_alternation_nfa(graph, start, accepting, nfa::anyList);
      break;
    default:
      build_character_nfa(graph, start, accepting, input[pointer]);
      break;
  }
  // We consume the character and return from the function
  ++pointer;
  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

void parse_character_class(const std::string& input, int& pointer,
                                std::vector<state>& graph, int* start,
                                int* accepting) {
  // Note that in our grammar definition, the character class production
  // is defined iteratively rather than recursively. We likewise will implement
  // it as such. We first determine if the starting character is a ^. If so
  // we have different implementations.

  if(pointer >= input.size()) {
    // We have reached the end of input unexpectedly
    throw malformed_pattern_exception(
        "Malformed Pattern Exception at position"
        + std::to_string(pointer - 1) +
        ": Reached end of input unexpectedly inside character class.");
  }
  if(input[pointer] == ']') {
    throw malformed_pattern_exception(
        "Malformed Pattern Exception at position "
        + std::to_string(pointer) +
        ": Character classes may not be empty.");
  }
  // In this event, we have a normal character class. We parse literals until
  // we reach a ']' character. As we parse literals we from the alternation of them.
  // In particular we initially add one accepting state and set the accepting
  // states of each nfa fragment to transition to the new accepting state.
  // Each time we parse a new literal, we add a start state with two epsilon
  // transitions. One goes to the start state of the fragment we are building
  // and the other goes to the start state of the nfa fragment of the newly
  // parsed literal.

  // We start by parsing the first literal
  int startState;
  int acceptingState;
  parse_literal(input, pointer, graph, &startState, &acceptingState);

  int tmpAcceptingState = graph.size();
  graph.push_back(state{nfa::kDeadStateTransition,-1,-1});
  graph[acceptingState] = state{nfa::kOneEpsilonTransition, tmpAcceptingState, -1};
  acceptingState = tmpAcceptingState;
  // While we have more input inside the character class, we parse a new fragment
  // and set up the appropriate transitions. The loop obeys the following
  // invariant. Upon entering the each iteration, startState and
  // acceptingState points to the startState and the acceptingStaten
  // of the fragment we are building respectively.
  while(pointer < input.size() && input[pointer] != ']') {
    // These are the start/accepting states of the newly parsed fragment
    int newStartState;
    int newAcceptingState;
    parse_literal(input, pointer, graph, &newStartState, &newAcceptingState);

    // We add a new start state
    int tmpStartState = graph.size();
    graph.push_back(state{nfa::kTwoEpsilonTransition, startState, newStartState});
    graph[newAcceptingState] = state{nfa::kOneEpsilonTransition, acceptingState, -1};

    startState = tmpStartState;
  }

  // We have either encountered a ']' or the end of input
  if(pointer >= input.size()) {
    // We have reached the end of input unexpectedly
    throw malformed_pattern_exception(
        "Malformed Pattern Exception at position"
        + std::to_string(pointer - 1) +
        ": Reached end of input unexpectedly inside character class.");
  }

  // We consume the ']' character and return from the function
  ++pointer;
  *start = startState;
  *accepting = acceptingState;
  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

void match(const std::string& input, int& pointer, char c) {
  // We first check if we have reached the end of input
  if(pointer >= input.size()) {
    throw malformed_pattern_exception(
        "Malformed Pattern Exception at "
        + std::to_string(pointer - 1) +
        ": Expected '" + c + "' but reached end of input prematurely.");
  } else if(input[pointer] != c) {
    throw malformed_pattern_exception(
        "Malformed Pattern Exception at "
        + std::to_string(pointer - 1) +
        ": Expected '" + c + "' but found '" + input[pointer] + "'.");
  } else {
    ++pointer;
  }
}

void build_character_nfa(std::vector<state>& graph, int* start,
                              int* accepting, char c) {
  *start = graph.size();
  *accepting = *start + 1;

  // NOTE: we first convert c to an unsigned char. The unsigned char is then
  // promoted to an int. The effect is that regardless of whether char is signed
  // or unsigned in a given implementation, transition will always be the expected
  // value from 0 to kMaxCharacterValue (inclusive).
  int transition = static_cast<unsigned char>(c);
  graph.push_back(state{transition, *accepting, -1});
  graph.push_back(state{nfa::kDeadStateTransition, -1, -1});

  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

// Builds an nfa that accepts any of the characters in array. Note that the
// array must be nonempty.
void build_alternation_nfa(std::vector<state>& graph, int* start,
                           int* accepting, const std::vector<char>& array) {
  // We will perform a similar construction to the one done to build a character
  // class. We iterate over the characters we want to accept in this nfa fragment
  // and we append starting states to the graph with the appropriate transitions
  int startState;
  int acceptingState;
  build_character_nfa(graph, &startState, &acceptingState, array[0]);
  int tmpAcceptingState = graph.size();
  graph.push_back(state{nfa::kDeadStateTransition, -1, -1});
  graph[acceptingState] = state{nfa::kOneEpsilonTransition, tmpAcceptingState, -1};
  acceptingState = tmpAcceptingState;
  
  // Invariant: startState always points to the startState of the nfa fragment
  // that we are building whenever we enter the loop.
  for(int i = 1; i != array.size(); ++i) {
    int newStartState;
    int newAcceptingState;
    build_character_nfa(graph, &newStartState, &newAcceptingState, array[i]);
    int tmpStartState = graph.size();
    graph.push_back(state{nfa::kTwoEpsilonTransition, startState, newStartState});
    graph[newAcceptingState] = state{nfa::kOneEpsilonTransition, acceptingState, -1};
    startState = tmpStartState;
  }
  // Now we set the start and accepting output
  *start = startState;
  *accepting = acceptingState;

  assert(graph[*accepting].transition == nfa::kDeadStateTransition);
}

// A helper function for the parse_term function
// Returns true if the character can be the first character in the production
// of a Term nonterminal.
bool is_term_prefix(char c) {
  switch(c) {
    case ')':
    case ']':
    case '*':
    case '+':
    case '?':
    case '^':
    case '|':
      return false;
    default:
      return true;
  }
}

// Note that in the grammar we specify a regex style production for the term
// nonterminal. Thus we take an iterative approach in this function.
// The following is a helper function that determines if a character
// is a * + or ?
bool is_unary_operator_character(char c) {
  switch(c) {
    case '*':
    case '+':
    case '?':
      return true;
    default:
      return false;
  }
}

// The following functions construct the static public arrays.
// We iterate over all characters. We only stop once we've reached one past
// the maximum character value. Note that we will use the std::isblank()
// functions from the cctype header. These functions accept an integer
// equivalent to the value of a char as an unsigned char.
// Thus we will use integers in our iteration.
std::vector<char> make_digit_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(std::isdigit(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_nondigit_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(! std::isdigit(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_word_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(std::isalnum(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_nonword_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(! std::isalnum(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_whitespace_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(std::isspace(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_nonwhitespace_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(! std::isspace(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_alphabet_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(std::isalpha(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_nonalphabet_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    if(! std::isalpha(character)) {
      list.push_back(static_cast<char>(character));
    }
  }
  return list;
}

std::vector<char> make_any_list() {
  std::vector<char> list;
  for(int character = 0; character != kNumberOfCharacters; ++character) {
    list.push_back(static_cast<char>(character));
  }
  return list;
}
} // unnamed namespace
} // namespace regex
} // namespace dynsolver
