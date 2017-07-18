#include "compiler/ast/factorial_operator_node.h"
factorial_operator_node::factorial_operator_node(expression_node* child) :
unary_operator_node(child) {

}

double factorial_operator_node::evaluate() const {
  unsigned long long val = static_cast<unsigned long long>(child->evaluate());
  if(val == 0) return 1;

  unsigned long long i = val - 1;
  while(i) {
    val *= i;
    --i;
  }
  return static_cast<double>(val);
}

std::ostream& factorial_operator_node::print(std::ostream& out) const {
  out << '(';
  child->print(out) << ')' << '!';
  return out;
}

std::ostream& factorial_operator_node::emit_code(std::ostream& acc) const {
  child->emit_code(acc);  //Put on %st(1)
  acc << "FACTORIAL %st(0), %st(1)\n";
  return acc;
}
