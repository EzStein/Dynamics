#ifndef DYNSOLVER_PARSER_PARSER_H_
#define DYNSOLVER_PARSER_PARSER_H_
namespace dynsolver {
namespace parser {

// This pure virtual class defines the interface for a parser.
// Multiple parsers implement this interface. The parser permits for an
// S-attributed style of syntax directed translation. That is, the nodes of the
// parse tree are traversed in post-order allowing for synthesized attributes.
// This can be implemented easily using bottom up parsers but is slightly
// more involved for non-recursive descent top-down parsers. Currently this parser
// generator does not support L-attributed syntax directed translation. That is,
// we do not visit parent nodes before determining their children. While it is
// possible to implement L-attributed SDT with a top-down parser and even with
// a bottom-up parser whose underlying grammar is LL(1), we will not implement
// this case yet.
//
// This pure virtual interface class has exactly one function. The function is
// parameterized by the type of object to which stores the grammar symbol
// attributes. This object is often a pointer to some base class type. Careful
// use of this pointer allows us to construct abstract syntax trees or to extract
// attribute imformation from the children. The function
// accepts a string to parse and a call back function pointer.
class parser {
 public:
  // We use a template type alias in order to specify an alias for the
  // call back function. callback_function_t is a pointer to a function that
  // returns a T and has 4 arguments. The first argument nonterminal indicates
  // which nonterminal represents the current parse node. The productionIndex
  // argument indicates which production of the nonterminal is used in the parse
  // tree. The first production has productionIndex 0, the second has 1 and so
  // on. The children argument is an array of T's specifying the nonterminal
  // children in the order that they appear in the production. The values in
  // this array are copied from the underlying container in the parser. It is
  // advisable that T be a pointer type to objects whose memory is managed
  // by the callback function and not by the parser itself. Finally, the
  // lexemes argument is a list of the terminal's lexemes as identified
  // by the lexer, presented in the order that they appear in the
  // production.
  template<class T>
  using callback_function_t =
      T (*)(int nonterminal, int productionIndex,
            const std::vector<T>& children,
            const std::vector<std::string>& lexemes);
  
  template<class T>
  void parse(const std::string& input, callback_function_t<T> callback) = 0;
};
}
}
#endif
