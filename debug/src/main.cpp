//#include <wx/wx.h>

//#include "debug/app.h"

//wxIMPLEMENT_APP(dynsolver::debug::app);

#include <iostream>
#include "compiler/expression_parser.h"
#include <climits>

int main(void) {
  dynsolver::compiler::expression_parser parse;
  std::list<symbol> symbolTable;
  symbolTable.push_back(symbol("x", 0, 0));
  std::cout << "Expression:" << std::endl;
  std::string expr;
  std::getline(std::cin, expr);
  std::cout << "Value:" << std::endl;
  double val;
  std::cin >> val;
  AST ast = parse.parse(expr, symbolTable);
  dynsolver::compiler::function<double, const double*> function = ast.compile();
    std::cout << "Result: " << function(&val) << std::endl;
  return 0;
}
