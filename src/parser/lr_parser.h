#ifndef DYNSOLVER_PARSER_LR_PARSER_H_
#define DYNSOLVER_PARSER_LR_PARSER_H_

#include "parser/common.h"
#include "parser/grammar.h"
#include "regex/lexer.h"

namespace dynsolver {
namespace parser {

// This class implements LR parsing. It can parse LR(0) SLR(1) LALR(1) and LR(1)
// grammars. It can be constructed from the grammar class.
class lr_table {
 public:
  // Parses the input provided by the lexer
  // using the grammar ruleset that was created at
  // construction. For now we will use
  // S-attributed syntax directed translation
  // scheme. That is, when we visit a node in the parse tree, we have access
  // to all of its children nodes (we traverse the parse tree post-order).
  // We can thus build up attributes for each node using a callback functor.
  //
  // In particular, we use type parameterization polymorphism so that any
  // functor object with the appropriate type can be used as a callback.
  // The callback
  // functor is called during the post order traversal of the parse tree. Each call
  // passes in the nonterminal symbol representing the subtree
  // that we are building, the production index which identifies which production
  // in the grammar was choosen as the handle, a list
  // of children attribute objects which represent
  // in order the nonterminals listed in
  // the production, and finally a list of strings which are the lexemes of the
  // terminal symbols in the order that they appear in the production. The call
  // back function must return a attribute object representing the subtree
  // just processed.
  // This child is passed into future calls of the function.
  // The attribute object of each node are parameterized by the type
  // ATTR_TYPE.
  //
  // This is a sample functor object that is admissable as a callback:
  //
  // node* my_callback(int nonterminal, int productionIndex,
  //                   const std::list<node*>& children,
  //                   const std::list<std::string>& lexemes)
  template <class ATTR_TYPE, class CALL_TYPE>
  void parse(const regex::lexer& lexer, CALL_TYPE callback);
  
 private:
  friend class grammar;
  // Only friend classes can construct instances of this object.
  // That is, only the grammar class can construct a parser.
  lr_parser();
  
  // grammarInfo maps a production to the number of grammar symbols in
  // in the production.
  // That is, it maps a nonterminal and its production index to
  // a production_info object containing the number of terminal
  // and nonterminal symbols.
  std::unordered_map<int, std::unordered_map<int, production_info> > 
  
  
  // Indicates the type of entry in the parse table.
  enum class entry_type {SHIFT, REDUCE, GOTO, ERROR};
  
  // Returns the entry type along with its data.
  //
  // If the entry type is SHIFT, then data refers to
  // which state is shifted onto the stack.
  //
  // If entry type is REDUCE, then next refers to the
  // nonterminal which the handle is reduced into
  // and productionIndex indicates which production
  // of that nonterminal is used.
  //
  // If GOTO is the type
  // next refers to the goto state.
  //
  // If the type is ERROR, then there is no entry
  // in the parse table.
  //
  // Precondition: the state must exist. (We may assert an error
  // if it does not).
  void get_table_entry(int state, int symbol, entry_type& entry,
                       int& next, int& productionIndex);

  // Returns the number of terminals and nonterminals associated with each
  // production.
  // Precondition: the nonterminal, productionIndex pair must exist.
  void get_grammar_info(int nonterminal, int productionIndex,
                        int& terminalSymbols, int& nonterminalSymbols);

  // Returns the list of tokens that can cause a shift or reduce without
  // an error on the given state. No check are performed to ensure that
  // state is an actual state.
  std::vector<int> get_expected_tokens(int state);
  
  // This class are the entries in the table.
  // See the documentation of get for what next and productionIndex
  // refer to. The table does not actually store entries of type error.
  struct table_entry {
    entry_type type;
    int next;
    int productionIndex;
  };
  
  // The parsing table is used to make parsing decisions.
  // It is implemented as a std::vector of maps. The vector is
  // indexed by states and the maps map grammar symbols to the
  // corresponding table entry for the state, grammar symbol pair.
  std::vector<std::unorderd_map<int, table_entry> > table;
};

// Algorithm:
// We first push the starting state onto the stack.
// We then check the input stream and consult the parsing table.
// There is either a syntax error in the stream, or we perform
// a shift/reduce. In a shift, we consume the lexeme and push its corresponding
// state onto the stack. In a reduce, we remove the some grammar symbols from
// the state stack and remove some (fewer) nonterminal attribute objects
// from the attribute stack. We then push the new state onto the stack
// and get a new attribute for that state from the callback function.
// We also maintain a lexeme stack indicating the lexemes of terminals thus far
// seen.
// INVARIANT: stateStack contains all grammar symbols. attributeStack contains
// those grammar symbols on the stateStack which are nonterminals while
// lexemeStack contains those which are terminals.
template <class ATTR_TYPE, class CALL_TYPE>
void lr_parser::parser(const lexer&, CALL_TYPE callback) {
  std::stack<int> stateStack;
  std::stack<ATTR_TYPE> attributeStack;
  std::stack<std::string> lexemeStack;
  
  // The start state is always 0.
  stateStack.push(0);
  while(!stateStack.empty()) {
    int state(stateStack.top());
    
    if(!lexer.has_next()) {
      // Premature end of input.
      throw syntax_exception(-1, get_expected_states(state),
                             syntax_exception::kEndPosition,
                             lexer.character_position(),
                             lexer.line_position(),
                             "");
    }
    
    std::string inputLexeme;
    int inputToken = lexer.peek_token(inputLexeme);
    
    int next, productionIndex;
    entry_type type;
    get_table_entry(state, inputToken, type, next,
                    productionIndex);
    
    assert(type != entry_type::GOTO);
    
    if(type = entry_type::ERROR) {
      throw syntax_exception(inputToken, get_expected_states(state),
                             lexer.token_position(),
                             lexer.character_position(),
                             lexer.line_position(),
                             inputLexeme);
    }
    
    if(type = entry_type::SHIFT) {
      lexer.consume_token(inputLexeme);
      stateStack.push(next);
      lexemeStack.push(lexeme);
    } else if(type = entry_type::REDUCE) {
      int nonterminalSymbols, terminalSymbols;
      get_grammar_info(next, productionIndex, terminalSymbols, nonterminalSymbols);
      std::list<ATTR_TYPE> childAttributes;
      std::list<std::string> lexemes;
      for(int i = 0; i != nonterminalSymbols; ++i) {
        stateStack.pop();
        childAttributes.push_front(attributeStack.top());
        attributesStack.pop();
      }
      for(int i = 0; i != terminalSymbols; ++i) {
        stateStack.pop();
        lexemes.push_front(lexemeStack.top());
        lexemeStack.pop();
      }
      // The result of the two previous for loops is that all the needed grammar
      // symbols (nonterminalSymbols + terminalSymbols) are poped off the stack.
      
      attributeStack.push(
          callback(next, productionIndex, childrenAttributes, lexemes));
      get_table_entry(
          stateStack.peek(), next, entry, next, productionIndex);
      assert(entry == entry_type::GOTO);
      stateStack.push(next);
    }
  }
  if(lexer.has_next()) {
    std::string inputLexeme;
    int inputToken = lexer.peek_token(inputLexeme);
    throw syntax_exception(inputToken, std::vector<int>(),
                           lexer.token_position(),
                           lexer.character_position(),
                           lexer.line_position(),
                           inputLexeme);
  }
}

std::vector<int> lr_parser::get_expected_states(int state) {
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
                                int& next, int& productionIndex) {
  assert(0 <= state && state < table.size);
  
  std::unordered_map<table_entry>::const_iterator iter =
      table[state].find(symbol);
  if(iter == table[state].end()) {
    entry = ERROR;
    return;
  }
  entry = iter->second.type;
  next = iter->second.next;
  productionIndex = iter->second.productionIndex;
}

void lr_parser::get_grammar_info(int nonterminal, int productionIndex,
                                 int& terminalSymbols, int& nonterminalSymbols) {
  auto iter1 = grammarInfo.find(nonterminal);
  assert(iter1 != grammarInfo.end());
  auto iter2 = iter1->second.find(productionIndex);
  assert(iter2 != iter1->second.end());
  terminalSymbols = iter2->second.terminalSymbols;
  nonterminalSymbols = iter2->nonterminalSymbols;
}


} // namespace parser
} // namespace parser

#endif
