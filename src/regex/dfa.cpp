#include "regex/dfa.h"

#include <cassert>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include "regex/common.h"
#include "regex/nfa.h"

namespace dynsolver {
namespace regex {

// This is used to contain helper objects for the dfa constructor
namespace {
// Used in the power set construction as the values of the map
// which associates sets to their DFA state.
struct set_info {
  int index;
  bool visited;
};

// Used in Hopcroft's algorithm to store bounds of classes in the partition.
// See description below.
struct index_pair {
  // The lower index which is inclusive.
  int low;
  // The higher index which is exclusive.
  int high;
};

// This is a helper function for Hopcroft's algorithm.
// It swaps the elements in the partion at indexA and indexB
// In doing so it also updates partitionInverse.
void swap(int* partition, int* partitionInverse,
          int indexA, int indexB) {
  int valA = partition[indexA];
  int valB = partition[indexB];
  partition[indexA] = valB;
  partition[indexB] = valA;
  partitionInverse[valA] = indexB;
  partitionInverse[valB] = indexA;
}
} // namespace anonymous

// Public member definitions

dfa::dfa(const std::string& pattern) {
  // This function first constructs an NFA from the pattern. We then build
  // the graph of this dfa using the powerset construction. Finally,
  // we minimize this dfa using Hopcroft's algorithm.
  nfa nfa(pattern);

  // Powerset Construction Algorithm: The algorithm works as follows. The states
  // of the DFA are sets of states of the NFA. We start with the state containing
  // the NFA starting state. We use the epsilon closure and transition function
  // to find all states reacheable over a given transition character. If we have
  // never seen this set of states before, we add a state to the dfa graph vector
  // and add a transition from the original state to the newly added one, of the
  // appropriate character. We do this for all characters. We then repeate the
  // process for all newly created states. The DFA accepting states are any
  // states that contain an NFA accepting state.
  //
  // We maintain both a stack and a map. The stack is a collection of
  // std::set<int> (a set of NFA states) that have yet to be processed.
  // It is initialized to contain the std::set containing just
  // the epsilon closure of the NFA start. While
  // the stack is nonempty, we pop an element off the stack. Using this element,
  // we add potentially kNumberOfCharacters
  // more elements (one corresponding to the set of
  // states reacheable through a given character transition). For each of the
  // kNumberOfCharacters added elements,
  // we lookup the index of the corresponding DFA state in
  // the map. If it does not already exist, we generate a DFA state and add the
  // set associated with it to the map. Finally, we add transitions from the
  // DFA state associated with the popped set to each of the
  // kNumberOfCharacters states
  // associated with the newly pushed sets. If the DFA state associated to the
  // popped set is accepting if the popped set contains an NFA accepting state.
  // Note that it is possible that one of the newly pushed sets will already
  // be in the stack, or will have already been processed by the stack. Thus
  // we associated to each set (using the same map as the index association)
  // a visited bool which is set to true once the set is processed by the stack.
  //
  // The map associates a std::set<int> representing a set of nfa states
  // to a set_info struct which exists in an anonymous namespace. The struct
  // has an index field storing the dfa state associated to it, and a
  // visited bool indicating whether the set has already been processed or not.
  //
  // Note that in order to prevent unnecessary redundent storage and copying,
  // the stack will actually contain iterators that point to key-value pairs
  // in the setInfoMap. The actual set objects will be stored in the map,
  // and the stack will simply contain references to them. Note that this
  // is a minor point and does not effect the algorithm.
  //
  // In Hopcroft's algorithm which follows we will need the inverse of
  // the graph variable. That is we will construct alongside graph,
  // graphInverse which maps state and character pairs to a list
  // of states which transition to the given one on the given character
  // See the next section for more details on graphInverse variable.
  std::stack<std::map<std::set<int>, set_info>::iterator> processingStack;
  std::map<std::set<int>, set_info > setInfoMap;
  std::vector<std::list<int> > graphInverse[kNumberOfCharacters];

  std::set<int> initialSet;
  nfa.epsilon_closure(std::set<int>{nfa.startState}, initialSet);
  startState = 0;
  // We check to see if this is an accepting state
  if(initialSet.find(nfa.acceptingState) != initialSet.end()) {
    graph.push_back(state(true));
  } else {
    graph.push_back(state(false));
  }
  setInfoMap.insert(std::make_pair(initialSet, set_info{0, false}));
  processingStack.push(setInfoMap.begin());
  
  // Invariant: All sets that are in the stack are the same as their epsilon
  // closure. Additionally, all sets in the stack have a mapping already defined
  // in setInfoMap (this is implicit since the stack actually stores iterators to
  // the setInfoMap) and an associated state in the graph vector.
  while(! processingStack.empty()) {
    std::map<std::set<int>, set_info>::iterator
        examinedSetIterator(processingStack.top());
    processingStack.pop();
    // If the state has already been visited, we skip it.
    if(examinedSetIterator->second.visited) {
      continue;
    }
    int dfaStateIndex = examinedSetIterator->second.index;
    for(int c = 0; c != kNumberOfCharacters; ++c) {
      char characterVal = static_cast<char>(c);
      std::set<int> tmpSet;
      std::set<int> setToAdd;
      nfa.transition_function(examinedSetIterator->first, characterVal, tmpSet);
      if(tmpSet.empty()) {
        // There are no transitions from this DFA state along this character
        graph[dfaStateIndex].transitions[c] = -1;
        continue; // We don't need to add anything to the map or stack
      }
      nfa.epsilon_closure(tmpSet, setToAdd);
      
      // We now lookup the index of the setToAdd in the dfa graph vector.
      // If it does not exist, we add an element to the dfa graph vector, and
      // to the setInfoMap.
      int index;
      std::map<std::set<int>, set_info>::iterator
          iter(setInfoMap.find(setToAdd));
      if(iter == setInfoMap.end()) {
        // The set has not yet been added to the map or the graph vector
        // We first check if it is an accepting state
        bool accepting;
        if(setToAdd.find(nfa.acceptingState) != setToAdd.end()) {
          accepting = true;
        } else {
          accepting = false;
        }
        index = graph.size();
        graph.push_back(state(accepting));
        // We will later add iter to the stack, so we update it with the
        // newly added set.
        iter = setInfoMap.insert(std::make_pair(setToAdd, set_info{index, false})).first;
      } else {
        // The set already exists in the map and the graph vector
        index = iter->second.index;
      }
      processingStack.push(iter);
      // We now set the transition from the examined dfa state
      graph[dfaStateIndex].transitions[c] = index;
      // We also update graphInverse.
      graphInverse[c][index].push_back(dfaStateIndex);
    }
    // We have finished processing the set so we mark it as visited
    examinedSetIterator->second.visited = true;
  }

  // We now have a DFA in the appropriate state. The startState variable
  // is set, and graph is populated with the appropriate values. However the DFA
  // may not be minimal. We use Hopcroft's algorithm to minimize it.
  //
  // Hopcroft's Algorithm: The algorithm functions by partitioning the set of
  // states into classes whose behavior is the same under simulation of all
  // strings. Each state in one class behaves the same as another
  // state in the same class in the sense that for all strings,
  // the simulation of the DFA starting in both states ends in
  // accepting states or rejecting states but not both.
  // We start with a partition of the states into two classes
  // which are certainly have different behavior, the accepting and the
  // rejecting states. We then refine the partition P as follows: choose
  // a class (an element), C, of the partition P
  // and form the pre-image of C
  // on a given transition character, c.
  // That is, we find the set of states for
  // which there exists a transition on c to an element of C.
  // We call C the splitter set and we call its pre-image D.
  // Each element in D
  // necessarily behaves differently than an element in D's complement
  // since on the character transition c, an element in D transitions
  // to an element in C which behaves differently than any other element.
  // Thus we can now split each of the classs in the partition P according
  // to D. For each class, B in the partition P, we split it into
  // B intersect D and B - D. For a given splitter C we do this process for
  // each character. Once we have exhausted all the characters, we choose
  // a new splitter and do the process again.
  // It can be shown that this process eventually terminates in a partition
  // representing a minimal DFA.
  // The algorithm is not quite complete. The choice of C the splitter is
  // somewhay important. We choose C from a subset of the partition P which
  // we shall call L. L must always be a subset of P. It is produced as follows.
  // When we choose C from L, we remove C from L. Whenever we split a class
  // B in P, we choose one of the two splits B' or B'' to add to L. However,
  // if L already contains B, then we add both of them (and remove B from L).
  // The algorithm terminates when there are no more splitters to choose from L.
  // That is, when L is empty.
  //
  // An efficient implementation of this algorithm is somewhat complicated.
  // The following implementation is based on the proposed implementation
  // in the article "Around Hopcroft's Algorithm" by Manuel Baclet and
  // Claire Pagetti. Note that my version deviates slightly from the proposed
  // implementation most notably in the numbering of indices as
  // half-open intervals in the partition array.
  // In this implementation, each class of the partition is assigned an index.
  // We need the following data structures:
  //
  // graphInverse - This is a std::vector<std::list<int> >[] that maps each
  //                DFA state and character pair to a list of states that 
  //                transition to it over that character. In particular,
  //                graphInverse[character][state] contains  implies that
  //                graph[val].transition[character] == state
  //
  // partition - A fixed size array of ints refering to all the states of the
  //             DFA. The ints are constantly permuted in the array so as to
  //             be consistent with the next data structure.
  //
  // partitionInverse - A fixed size array which is the inverse of the partition
  //                    array. That is, if partition[a] = b then
  //                    partitionInverse[b] = a
  //
  // classIndices - A std::vector of index_pair. The indices of this std::vector
  //                are the classes of the partition. The elements of the class
  //                with indexPair (low, high) are the elements of partition with
  //                indices between low (inclusive) and high (exclusive).
  //
  // classSplitPointer - This is a std::vector of ints which maps the classes
  //                     to an index, i, in the partition array. If l and h
  //                     denote the bounds of the class in the partition array,
  //                     l (inclusive) to i (exclusive) denote the part of the
  //                     class that (definitely)
  //                     has elements in common with the pre-image of the splitter.
  //                     Likewsie, i (inclusive) to h (exclusive) indicate the
  //                     other split of the class. Note that this variable
  //                     is used for intermediate results, is updated as we
  //                     iterate through the pre-image of the splitter.
  //
  // classArray - A fixed size array of ints that map each state of the DFA to
  //              its class (that is, to its index in classIndices).
  //
  // splitters - A stack of ints refering to classes in P. This stack is used
  //             to select the next splitter which will always be the one most
  //             recently added.
  //
  // visitedClasses - A std::vector of ints which holds the classes that we
  //                  may need to split (the classes with a nonempty intersection
  //                  with the pre-image of the splitter).
  // 
  // splitterIntersection
  //             - A std::vector which holds the elements of splitter which
  //             are in common with the pre-image of splitter. This is necessary
  //             since we are iterating over the elements of splitter and thus
  //             cannot modify it immediately.
  //
  // We start with a partition of two classes, the accepting and rejecting
  // states. While splitters is not empty, we pop splitters into splitter
  // and for each character we do the following:
  // We permute the elements of each class
  // so that the left side of the class contains elements in common with
  // the pre-image of the splitter. We update the classSplitPointer
  // accordingly. We accomplish this as follows:
  // We iterate through the pre-image of splitter.
  // On each DFA state in the iteration, we identify which class it is in.
  // This indicates that the class has a nonempty intersection with the
  // pre-image. We then swap that element with the element pointed to
  // by the classSplitPointer array and we increment this pointer.
  // After iterating through all elements, the pointer will indicate where
  // the split of the class would occur. Note however that since we are
  // iterating over the splitter, we cannot simply swap elements in the
  // splitter (it would screw up the iteration order). Instead we store those
  // elements of the splitter witch are in common to the splitter's pre-image
  // in splitterIntersection. Once the iteration is complete, we process the
  // splitter class just like the otherse using the splitterIntersection info.
  // We now have permuted the elements of each class so that they are ready
  // to be split. As an optimization, we do not need to split classes which
  // we did not actually permute, so we maintain a visitedClasses variable which
  // contains the classes which potentially need to be split. Classes of one element
  // cannot be split so are also ommitted from visitedClasses.
  //
  // We can now perform the partition refinement. For each class in visitedClasses
  // we check to see if the class needs to be split. It only needs to be split
  // if the classSplitPointer does not point to either the high or low endpoint
  // of the class. After checking this condition, we compute which of the two
  // class sections is smaller. We add a new class to the stack and to the
  // classIndices array whose indices are indicate the smaller section. We
  // also update the old class's indices to reflect those of the longer section.
  // We must also reset the classSplitPointer array of the changed classes
  // to the lower endpoint index.
  //
  // Running this algorithm on all character/splitter pairs until the
  // splitter stack is empty produces a partition of the old DFA where
  // all transitions on a given character from an element of a class
  // go to (potentiall different) elements of a single (possibly different) class.
  // We can recover the new optimized DFA by assigning a state to each class
  // and a transition from class to class according to how its elements
  // transition.

  int* partition = new int[graph.size()];
  int* partitionInverse = new int[graph.size()];
  int* classArray = new int[graph.size()];
  std::vector<index_pair> classIndices;
  std::vector<int> classSplitPointer;
  std::stack<int> splitters;

  // We first setup two classes. One for the accepting and one for the rejecting
  // states. We iterate through the graph adding accepting states to the front
  // and rejecting states to the back. While this can be done during the
  // powerset construction, we do it here for clarity. This may change later.
  // We will let the accepting class be class 0 and the rejecting class be
  // class 1.
  int front(-1);
  int back(graph.size());
  for(int index = 0; index != graph.size(); ++index) {
    // Note that prefix operators increment the variable before
    // returning it's result.
    if(graph[index].accepting) {
      partition[++front] = index;
      partitionInverse[index] = front;
      classArray[index] = 0;
    } else {
      partition[--back] = index;
      partitionInverse[index] = back;
      classArray[index] = 1;
    }
  }
  // Sanity Check
  assert(front + 1 == back);
  // Front is now the index in the partition of the last accepting state and
  // back is the index of the first rejecting state.
  // The indices of the accepting class:
  classIndices.push_back(index_pair{0, back});
  // Indices of the rejecting class:
  classIndices.push_back(index_pair{back, graph.size()});

  // We finally initialize the classSplitPointer of both classes
  // to be the low endpoint of the two classes.
  classSplitPointer.push_back(0);
  classSplitPointer.push_back(back);

  // To kick off the algorithm, we add the smaller of the two classes to the
  // splitter stack. The variable back is the size of the accepting states
  // and value graph.size() - back is the size of the rejecting states.
  if(back < graph.size() - back) {
    // Push the accepting class
    splitters.push(0);
  } else {
    // Push the rejecting class
    splitters.push(1);
  }

  // Main algorithm loop
  while(! splitters.empty()) {
    int splitter(splitters.top());
    splitters.pop();
    // We iterate over all characters
    for(int character = 0; character != kNumberOfCharacters; ++character) {
      std::vector<int> visitedClasses;
      std::vector<int> splitterIntersection;

      // We now iterate through the pre-image of the splitter
      for(int splitterElement = classIndices[splitter].low;
          splitterElement != classIndices[splitter].high;
          ++splitterElement) {
        for(std::list<int>::const_iterator preimageElement =
                graphInverse[character][splitterElement].begin();
            preimageElement != graphInverse[character][splitterElement].end();
            ++preimageElement) {
          // preimageElement will eventually reach every element in the preimage
          // of the splitter class. We define the following variables
          // which will all be specific to the preimageElement we are considering.
          int preimageClass = classArray[*preimageElement];
          int classLowIndex = classIndices[preimageClass].low;
          int classHighIndex = classIndices[preimageClass].high;
          int preimagePartitionIndex = partitionInverse[*preimageElement];
          int preimageClassSplitPointer = classSplitPointer[preimageClass];
          
          // If the class is a single element, it cannot be split, so we skip it
          if(classLowIndex + 1 == classHighIndex) {
            continue;
          }
          
          // Now we attempt to permute the elements of the class
          if(preimageClass == splitter) {
            // Since we are iterating over the splitter we must treat it
            // by adding the preimageElement to the splitterIntersection
            // vector for later processing.
            // We will add splitter to the visited classes list later.
            splitterIntersection.push_back(*preimageElement);
          } else {
            // If we haven't seen this class before then we add it to the
            // visited classes list. We haven't seen this class yet if the
            // classSplitPointer points to the low index.
            if(preimageClassSplitPointer == classLowIndex) {
              visitedClasses.push_back(preimageClass);
            }
            // Whether or not we have seen it before, permute the class so
            // that the preimageElement is at the begining. We do this by
            // swapping the preimageElement and the element at classSplitPointer
            // This helper function also updates partitionInverse.
            swap(partition, partitionInverse,
                 preimagePartitionIndex, preimageClassSplitPointer);
            ++classSplitPointer[preimageClass];
          }
        }
      }
      // Now that we are done iterating through the splitter, we process it
      // correctly.
      if(! splitterIntersection.empty()) {
        visitedClasses.push_back(splitter);
        for(std::vector<int>::const_iterator iter = splitterIntersection.begin();
            iter != splitterIntersection.end(); ++iter) {
          int preimagePartitionIndex = partitionInverse[*iter];
          int preimageClassSplitPointer = classSplitPointer[splitter];
          swap(partition, partitionInverse,
               preimagePartitionIndex, preimageClassSplitPointer);
          ++classSplitPointer[splitter];
        }
      }

      // We are now ready to actually split the classes
      for(std::vector<int>::const_iterator classIter = visitedClasses.begin();
          classIter != visitedClasses.end(); ++classIter) {
        // If the entire class is the intersection we don't need to split
        int splitPointer = classSplitPointer[*classIter];
        int lowIndex = classIndices[*classIter].low;
        int highIndex = classIndices[*classIter].high;
        if(splitPointer == highIndex) {
          continue;
        }
        // We compute which fragment is smaller. The smaller fragment becomes
        // the new class.
        int newClass = classIndices.size();
        if(splitPointer - lowIndex < highIndex - splitPointer) {
          // The first section is smaller.
          classIndices.push_back(index_pair{lowIndex, splitPointer});
          classIndices[*classIter] = index_pair{splitPointer, highIndex};
          classSplitPointer.push_back(lowIndex);
          classSplitPointer[*classIter] = splitPointer;
          // We need to update the class array. Every item in the first fragment
          // now belongs to a new class.
          for(int i = lowIndex; i != splitPointer; ++i) {
            classArray[partition[i]] = newClass;
          }
        } else {
          classIndices.push_back(index_pair{splitPointer, highIndex});
          classIndices[*classIter] = index_pair{lowIndex, splitPointer};
          classSplitPointer.push_back(splitPointer);
          classSplitPointer[*classIter] = lowIndex;
          // We need to update the class array. Every item in the second fragment
          // now belongs to a new class.
          for(int i = splitPointer; i != highIndex; ++i) {
            classArray[partition[i]] = newClass;
          }
        }
        splitters.push(newClass);
      }
    }
  }

  // We have completed Hopcroft's algorithm. We can now build a new minal DFA
  // graph. We iterate over all the classes, adding them to the minimal graph
  // and filling in the transitions.
  std::vector<state> minimalGraph;
  minimalGraph.reserve(classIndices.size());
  for(int classVal = 0; classVal != classIndices.size(); ++classVal) {
    int representativeState = partition[classIndices[classVal].low];
    for(int character = 0; character != kNumberOfCharacters; ++character) {
      minimalGraph[classVal].transitions[character]
          = classArray[graph[representativeState].transitions[character]];
    }
    minimalGraph[classVal].accepting = graph[representativeState].accepting;
  }
  delete[] partition;
  delete[] partitionInverse;
  delete[] classArray;

  std::swap(minimalGraph, graph);
  startState = classArray[startState];
}

int dfa::accept_longest_prefix(const std::string& candidate,
                               int startPosition) const {
  assert(startPosition >= 0 && startPosition <= candidate.size());
  // We simulate the dfa until termination. We terminate when we either
  // reach a state with no transition for a particular character, or
  // we reach the end of input. As we simulate the DFA we keep track of the
  // longest prefix which admitted an accepting state. Initially this
  // value is set to -1, but if the startState is accepting, then we set it to 0.
  // We update the value every time we reach an accepting state.
  int longestPrefixSeen(-1);
  int pointer(startPosition);

  if(graph[startState].accepting) {
    longestPrefixSeen = 0;
  }

  int currentState = startState;
  // Loop invariant: upon entering each iteration of the loop, currentState
  // indicates the index of the state we are in before consuming the next
  // character in the input
  while(pointer != candidate.size()) {
    int transitionCharacter = static_cast<unsigned char>(candidate[pointer]);
    currentState = graph[currentState].transitions[transitionCharacter];
    if(currentState == -1)
      break; // There was no transition for the given character
    if(graph[currentState].accepting) {
      // + 1 since we return the length of the prefix up to and including pointer
      longestPrefixSeen = pointer + 1;
    }
    ++pointer;
  }
  return longestPrefixSeen;
}

} // namespace dynsolver
} // namespace regex
