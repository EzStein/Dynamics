#include "compiler/expression_parser.h"

#include "regex/lexer_builder.h"
#include "util/util.h"

namespace dynsolver {
namespace compiler {

expression_parser::expression_parser() :
    lexer(
        regex::lexer_builder()
        .register_rule("\\s*\\+\\s*", PLUS)
        .register_rule("\\s*-\\s*", MINUS)
        .register_rule("\\s*\\*\\s*", TIMES)
        .register_rule("\\s*/\\s*", DIVIDE)
        .register_rule("\\s*^\\s*", CARET)
        .register_rule("\\s*,\\s*", COMMA)
        .register_rule("\\s*\\(\\s*", LEFT_PARENTHESIS)
        .register_rule("\\s*\\)\\s*", RIGHT_PARENTHESIS)
        .register_rule("\\s*(E|PI)\\s*", CONSTANT)
        .register_rule("\\s*(sin|cos|tan|log)\\s*", FUNCTION)
        .register_rule("\\s*\\a\\d*\\s*", VARIABLE)
        .register_rule("\\s*\\d\\d*(\\.\\d\\d*)?\\s*", NUMBER)
        .build()),
    parser(
        parser::grammar()
        .add_production(EXPRESSION)
        .append_symbol(EXPRESSION)
        .append_symbol(PLUS)
        .append_symbol(TERM)
        .done()
        
        .add_production(EXPRESSION)
        .append_symbol(EXPRESSION)
        .append_symbol(MINUS)
        .append_symbol(TERM)
        .done()
        
        .add_production(EXPRESSION)
        .append_symbol(TERM)
        .done()
        
        .add_production(TERM)
        .append_symbol(TERM)
        .append_symbol(TIMES)
        .append_symbol(FACTOR)
        .done()
        
        .add_production(TERM)
        .append_symbol(TERM)
        .append_symbol(DIVIDE)
        .append_symbol(FACTOR)
        .done()
        
        .add_production(TERM)
        .append_symbol(FACTOR)
        .done()
        
        .add_production(FACTOR)
        .append_symbol(BASE)
        .append_symbol(CARET)
        .append_symbol(FACTOR)
        .done()
        
        .add_production(FACTOR)
        .append_symbol(MINUS)
        .append_symbol(FACTOR)
        .done()
        
        .add_production(FACTOR)
        .append_symbol(BASE)
        .done()
        
        .add_production(BASE)
        .append_symbol(FUNCTION)
        .append_symbol(LEFT_PARENTHESIS)
        .append_symbol(PARAMETER_LIST)
        .append_symbol(RIGHT_PARENTHESIS)
        .done()
        
        .add_production(BASE)
        .append_symbol(LEFT_PARENTHESIS)
        .append_symbol(EXPRESSION)
        .append_symbol(RIGHT_PARENTHESIS)
        .done()
        
        .add_production(BASE)
        .append_symbol(NUMBER)
        .done()
        
        .add_production(BASE)
        .append_symbol(CONSTANT)
        .done()

        .add_production(BASE)
        .append_symbol(VARIABLE)
        .done()
        
        .add_production(PARAMETER_LIST).done()
        
        .add_production(PARAMETER_LIST)
        .append_symbol(NONEMPTY_PARAMETER_LIST)
        .done()
        
        .add_production(NONEMPTY_PARAMETER_LIST)
        .append_symbol(EXPRESSION)
        .append_symbol(COMMA)
        .append_symbol(NONEMPTY_PARAMETER_LIST)
        .done()

        .add_production(NONEMPTY_PARAMETER_LIST)
        .append_symbol(EXPRESSION)
        .done()
        .set_start(EXPRESSION)
        .generate_slr()) { }

::AST expression_parser::parse(const std::string& input,
                               const std::list<::symbol>& syms) {
  symbols = &syms;
  lexer.set_input(input);
  ::AST ast(parser.parse<expression_node*>(lexer, *this));
  return ast;
}

// This constructs an abstract syntax tree based on the production and the
// children and lexemes of a given handle.
expression_node* expression_parser::operator()(
    int nonterminal, int productionIndex, const std::list<expression_node*>& children,
    const std::list<std::string>& lexemes) {
  switch(nonterminal) {
    case EXPRESSION:
      switch(productionIndex) {
        case 0: {
          return ::AST::make_binary_operator_node<::binary_addition_operator_node>
              (*children.begin(), *(++children.begin()));
        } break;
        case 1: {
          return ::AST::make_binary_operator_node<::binary_subtraction_operator_node>
              (*children.begin(), *(++children.begin()));
        } break;
        case 2: {
          return *children.begin();
        } break;
        default: assert(false);
      } break;
    case TERM:
      switch(productionIndex) {
        case 0: {
          return ::AST::make_binary_operator_node<::binary_multiplication_operator_node>
              (*children.begin(), *(++children.begin()));
        } break;
        case 1: {
          return ::AST::make_binary_operator_node<division_operator_node>
              (*children.begin(), *(++children.begin()));
        } break;
        case 2: {
          return *(children.begin());
        } break;
        default: assert(false);
      } break;
    case FACTOR:
      switch(productionIndex) {
        case 0: {
          return ::AST::make_binary_operator_node<exponentiation_operator_node>
              (*children.begin(), *(++children.begin()));
        } break;
        case 1: {
          return ::AST::make_unary_operator_node<::unary_minus_operator_node>
              (*children.begin());
        } break;
        case 2: {
          return *(children.begin());
        } break;
        default: assert(false);
      } break;
    case BASE:
      switch(productionIndex) {
        case 0: {
          assert(false);
        } break;
        case 1: {
          return *children.begin();
        } break;
        case 2: {
          std::string lexeme = *(lexemes.begin());
          util::strip_white_space(lexeme);
          if(util::is_integer(lexeme)) {
            return ::AST::make_integer_number_leaf_node(
                util::string_to_long(lexeme));
          } else {
            return ::AST::make_number_leaf_node(util::string_to_double(lexeme));
          }
        } break;
        case 3: {
           assert(false);
        } break;
        case 4: {
          std::string lexeme = *(lexemes.begin());
          util::strip_white_space(lexeme);
          for(std::list<::symbol>::const_iterator symbol = symbols->begin();
              symbol != symbols->end(); ++symbol) {
            if(symbol->name == lexeme) {
              return ::AST::make_variable_leaf_node(symbol);
            }
          }
          assert(false);
        } break;
        default: assert(false);
      } break;
    case PARAMETER_LIST:
      assert(false);
      switch(productionIndex) {
        case 0: {
          
        } break;
        case 1: {
          
        } break;
        default: assert(false);
      } break;
    case NONEMPTY_PARAMETER_LIST:
      assert(false);
      switch(productionIndex) {
        case 0: {
          
        } break;
        case 1: {
          
        } break;
        default: assert(false);
      } break;
    default: assert(false);
  }
  return nullptr;
}
} // namespace compiler
} // namespace dynsolver
