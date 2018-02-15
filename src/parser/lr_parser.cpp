#include "parser/lr_parser.h"

#include <cassert>
#include <list>
#include <stack>
#include <unordered_map>

#include "parser/common.h"
#include "regex/lexer.h"
#include "parser/syntax_exception.h"

namespace dynsolver {
namespace parser {

lr_parser::lr_parser() { }

std::vector<int> lr_parser::get_expected_tokens(int state) const {
  assert(0 <= state && state < table.size());
  std::vector<int> expectedStates;
  auto iter = table[state].begin();
  auto end = table[state].end();
  for(; iter != end; ++iter) {
    if(iter->second.type != entry_type::GOTO) {
      expectedStates.push_back(iter->first);
    }
  }
  return expectedStates;
}

void lr_parser::get_table_entry(int state, int symbol, entry_type& entry,
                                int& next, int& productionIndex) const {
  assert(0 <= state && state < table.size());
  
  std::unordered_map<int,table_entry>::const_iterator iter =
      table[state].find(symbol);
  if(iter == table[state].end()) {
    entry = entry_type::ERROR;
    return;
  }
  entry = iter->second.type;
  next = iter->second.next;
  productionIndex = iter->second.productionIndex;
}

void lr_parser::get_grammar_info(int nonterminal, int productionIndex,
                                 int& terminalSymbols, int& nonterminalSymbols) const {
  auto iter1 = grammarInfo.find(nonterminal);
  assert(iter1 != grammarInfo.end());
  auto iter2 = iter1->second.find(productionIndex);
  assert(iter2 != iter1->second.end());
  terminalSymbols = iter2->second.terminalSymbols;
  nonterminalSymbols = iter2->second.nonterminalSymbols;
}
} // namespace parser
} // namespace dynsolver
