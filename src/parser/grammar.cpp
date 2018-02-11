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

// We must construct the parsing table and the grammar info table.
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
  for(auto iter = initProductions->begin();
      iter != initProductions->end(); ++iter) {
    initial.insert(item{start,*iter,0});
  }
  closure(initial);
  stateStack.push(initial);
  int indexCounter = 0;
  statesMap[initial] = 0;
  // We now begin the main loop.
  while(!stateStack.empty()) {
    std::set<item> set(stateStack.peek());
    stateStack.pop();
    int index = statesMap
    
    for(auto symbol = symbols.begin(); symbol != symbols.end(); ++symbol) {
      std::set<item> newState(move_to(set, *symbol));
      if(statesMap.find(newState) != statesMap.end()) {
        statesStack.push(newState);
        statesMap.insert[set]=++indexCounter;
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
        ret.insert(item{item->head, item->body, item->pointer+1});
      }
    }
  }
  closure(ret);
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
          for(auto prod = productions->begin();
              prod != productions->end();
              ++prod) {
            news ||= next.insert(item{head,*prod,0}).T2;
          }
        }
      }
    }
    std::swap(set, next);
  }
}
} // namespace parser
} // namespace dynsolver
