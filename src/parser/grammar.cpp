#include "parser/grammar.h"

#include <map>
#include <vector>

namespace dynsolver {
namespace parser {

// Constructs a grammar with no rules. We construct its productionBuilder
// object with a reference to this grammar.
grammar::grammar() : productionBuilder(*this) { }

production_builder& add_production(int nonterminal) {
  symbols.push_back(nonterminal);
  start = nonterminal;
  
  // If there is no entry for nonterminal in the map, this line
  // creates one and adds an (as of now) empty production to it.
  // If it already exists then we append an empty production.
  std::vector<std::vector<int> >& productions = grammarTable[nonterminal];
  productions.push_back(std::vector<int>());

  // We now set the productionBuilder so that it will modify the newly added
  // production.
  productionBuilder.productionArrayPointer =
      &(productions.back());
  
  return productionBuilder;
}

grammar::production_builder::production_builder(grammar& _ref) : ref(_ref) { }

production_builder& grammar::production_builder::append_symbol(int symbol) {
  ref.symbols.push_back(symbol);
  productionArrayPointer->push_back(symbol);
  return *this;
}

grammar& grammar::production_builder::done() {
  return ref;
}

bool grammar::is_terminal(int symbol) {
  return grammarTable.find(symbol) == grammarTable.end();
}

bool grammar::is_nonterminal(int symbol) {
  return !is_terminal(symbol);
}

namespace {
// Adds each element of src into dest. Returns true if dest changed as a result.
bool merge(std::set<int>& dest, const std::set<int>& src) {
  bool changed = false;
  for(auto iter = src.begin(); iter != src.end(); ++iter) {
    changed ||= dest.insert(*iter).T2;
  }
  return changed;
}
} // namespace anonymous

// Algorithm: If symbol is a terminal symbol then first contains just that
// symbol. If symbol is a nonterminal, we iterate over each of its productions
// and over each of the symbols in each production adding the first set of
// each symbol iff all previous symbols derive epsilon. Return true only if
// symbol derives epsilon.
// Follow is defined for nonterminal symbols. If there is a production (anywhere)
// where a grammar symbol B follows a nonterminal A, everything in FIRST(B) is
// in FOLLOW(A). If A appears at the end of a production or if B derives epsilon
// then everything in FOLLOW(HEAD) is in FOLLOW(A) where HEADis the head of the
// production.
//
// Implementation: The obvious implementation using recursion will not work when
// there are circular dependencies on the follow sets. Instead we iterate over
// each symbol and populate the firstFollowMap. We repeate this process until
// there is no change in the firstFollowMap.
void grammar::compute_first_follow() const {
  bool updated(true);
  while(updated) {
    updated = false;
    for(auto symbol = symbols.begin(); symbol != symbols.end(); ++symbol) {
      auto productions = grammarTable.find(*symbol);
      if(productions == grammarTable.end()) {
        // Symbol is a terminal, we update the first but not follow entries.
        firstFollowMap[symbol].epsilon = false;
        firstFollowMap[symbol].first.insert(symbol);
      }
      bool epsilon = false;
      for(auto production = productions->second.begin();
          production != productions->second.end();
          ++production) {
        bool epsilonBreak(false);
        for(auto iter = production.begin(); iter != production.end(); ++iter) {
          if(!epsilonBreak) {
            updated ||= merge(firstFollowMap[symbol].first, firstFollowMap[*iter].first);
            if(firstFollowMap[*iter].epsilon) {
              // If it doesn't derive epsilon, we do not add any of the other
              // elements.
              epsilonBreak = true;
            }
          }
          if(is_nonterminal(*iter)) {
            if(iter + 1 == production.end()) {
              updated ||= merge(firstFollowMap[*iter].follow,
                                firstFollowMap[symbol].follow);
            } else {
              updated ||= merge(firstFollowMap[*iter].follow,
                                firstFollowMap[*(iter+1)].first);
              if(firstFollowMap[*(iter+1)].epsilon) {
                updated ||= merge(firstFollowMap[*iter].follow,
                                  firstFollowMap[symbol].follow);
              }
            }
          }
        }
        if(!epsilonBreak) {
          updated ||= !firstFollowMap[symbol].epsilon;
          firstFollowMap[symbol].epsilon = true;
        }
      }
    }
  }
  return epsilon;
}

std::set<int> grammar::first(int symbol) const {
  return firstFollowMap.find(symbol)->second->first;
}

std::set<int> grammar::follow(int symbol) const {
  return firstFollowMap.find(symbol)->second->follow;
}

// We must construct the parsing table and the grammar info table.
// After constructing the grammar info table we construct the set of
// LR(0) items and the parsing table.
lr_parser grammar::generate_lr0() const {
  lr_parser parser;
  for(auto iter = grammarTable.begin();
      iter != grammarTable.end(); ++iter) {
    int productionIndex = 0;
    for(auto productionIter = iter->second.begin();
        productionIter != iter->second.end(); ++productionIter) {
      int terminalSymbols = 0;
      int nonterminalSymbols = 0;
      for(auto symbol = productionIter->second.begin();
          symbol != productionIter->second.end(); ++symbol) {
        if(is_terminal(*symbol)) {
          // Symbol not found so it is a terminal
          ++terminalSymbols;
        } else {
          ++nonterminalSymbols;
        }
      }
      parser.grammarInfo[iter->first][productionIter->first]
          = production_info{terminalSymbols, nonterminalSymbols};
    }
  }

  // This is used to process all the states. Unprocessed states
  // are pushed onto the stack and processed ones are popped off.
  std::stack<std::set<item> > stateStack;

  // Associates each state to its index. This also ensures that
  // we don't add already processed states back onto the stack.
  std::map<std::set<item>, int> statesMap;
  
  // We form the set of initial items and then take its closure.
  std::set<item> initial;
  auto initProductions = grammarTable.find(start);
  int counter = -1;
  for(auto iter = initProductions->begin();
      iter != initProductions->end(); ++iter) {
    initial.insert(item{start,*iter,0,++counter});
  }
  
  stateStack.push(initial);
  int indexCounter = 0;
  statesMap[initial] = 0;
  parser.table.push_back(std::unordered_map<int, entry_type>());
  // We now begin the main loop.
  // INVARIANT: Any states  that are on the stack have an entry in the stateMap.
  // We only store the kernal states on the stack. The other states can be constructed
  // from the closure. States in the stack also have an entry already created in the
  // parse table.
  while(!stateStack.empty()) {
    std::set<item> currentState(stateStack.top());
    stateStack.pop();
    int index = statesMap[currentState];
    std::set<item> closure(closure(currentState));
    // We populate the reduce elements of the parse table by iterating over each
    // item in the current state.
    for(auto item = currentState.begin(); item != currentState.end(); ++item) {
      if(item.pointer == item.body.size()) {
        // We add an entry for each entry (in follow item.head).
        std::set<int> follow(follow(item.head));
        for(auto symbol = follow.begin; symbol != follow.end(); ++symbol) {
          // If there is already an entry, then there is a reduce reduce conflict
          if(parser.table[index].find(*symbol) != parser.table[index].end()) {
            throw malformed_grammar_exception("Reduce Reduce conflict");
          }
          parser.table[index][*symbol] = table_entry{
            entry_type::REDUCE, item.head, item.productionIndex}
          }
        }
      }
    }
    
    // For each grammar symbol
    for(auto symbol = symbols.begin(); symbol != symbols.end(); ++symbol) {
      std::set<item> newState(move_to(closure, *symbol));
      if(!newState.empty()) {
        if(statesMap.find(newState) == statesMap.end()) {
          statesStack.push(newState);
          statesMap[newState]=++indexCounter;
          parser.table.push_back(std::unordered_map<int, table_entry>());
        }
        // We now add a shift action to the lr table if the symbol is a
        // a terminal, and a goto action if it is a nonterminal.
        // We first check to see if there is already an entry.
        if(parser.table[index].find(*symbol) != parser.table[index].end()) {
          throw malformed_grammar_exception("SHIFT REDUCE Conflict");
        }
        int newStateIndex = statesMap.find(newState)->second;
        if(is_terminal(*symbol)) {
          parser.table[index][*symbol] = table_entry{
            entry_type::SHIFT, newStateIndex,0,0
          };
        } else {
          parser.table[index][*symbol] = table_entry{
            entry_type::GOTO, newStateIndex,0,0
          };
        }
      }
    }
  }
  return parser;
}

std::set<item> move_to(const std::set<item>& set, int symbol) const {
  std::set<item> ret;
  for(auto item = set.begin(); item != set.end(); ++item) {
    if(item->pointer != item->body.size()) {
      if(item->body[item->pointer] == symbol) {
        ret.insert(item{item->head, item->body, item->pointer+1, item->productionIndex});
      }
    }
  }
  return ret;
}

void grammar::closure(std::set<item>& set) const {
  std::set<item> next(set);
  bool news(true);
  while(news) {
    news = false;
    for(auto iter = set.begin(); iter != set.end(); ++iter) {
      next.insert(*iter);
      if(iter->pointer != iter->body.size()) {
        int head = iter->body[iter->pointer];
        auto productions = grammarTable.find(head);
        if(productions != grammarTable.end()) {
          // We iterate over the list of productions for the
          // nonterminal that we found.
          int counter = 0;
          for(auto prod = productions->begin();
              prod != productions->end();
              ++prod) {
            news ||= next.insert(item{head,*prod,0,++counter}).T2;
          }
        }
      }
    }
    std::swap(set, next);
  }
}
} // namespace parser
} // namespace dynsolver
