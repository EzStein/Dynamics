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

std::ostream& factorial_operator_node::emit_code(std::ostream& acc, unsigned char * buf, unsigned int & offset) const {
  child->emit_code(acc, buf, offset);  //Put on %st(0)

  /*acc << "fist %eax\n"; //Round to an integer

  //TAKE ABS of %eax and store it in eax
  acc << "movl %eax, %ebx\n"; //Copy to eax
  acc << "sarl 31, %ebx\n"; //Arithmetic right shift
  acc << "xorl %ebx, %eax\n"; //Take two's complement
  acc << "subl %ebx, %eax\n"

  acc << "movl %eax, %ebx\n";

  acc << "mul"*/
  acc << "FACTORIAL %st(0), %st(1)\n";
  return acc;
}

unsigned int factorial_operator_node::code_size() const {
  return child->code_size() + 0;
}
