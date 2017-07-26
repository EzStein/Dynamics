#include <cmath>
#include "exponentiation_operator_node.h"
exponentiation_operator_node::exponentiation_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double exponentiation_operator_node::evaluate() const {
  return pow(leftChild->evaluate(), rightChild->evaluate());
}

std::ostream& exponentiation_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '^' << '(';
  rightChild->print(out) << ')';
  return out;
}

std::ostream& exponentiation_operator_node::emit_code(std::ostream& acc, unsigned char * buf, unsigned int & offset) const {

  rightChild->emit_code(acc, buf, offset); //Now on %st(1) EXPONENT
  leftChild->emit_code(acc, buf, offset);  //Put on %st(0) BASE
  acc << "fyl2x\n";
  //Performs exponent * log2 (base) stores in %st(0)
  //We must now perform 2^%st(0)
	//We do this by performing 2^((int)%st0 + (frac)%st0)
	//Or 2^intval * 2^fracval
  buf[offset] = '\xD9';
  buf[++offset] = '\xF1';

	acc << "fld %st(0)\n";	//Copies the value, Now it is held in %st(0) and %st(1)
  buf[++offset] = '\xD9';
  buf[++offset] = '\xC0';

	acc << "frndint\n";		//%st(0) now holds the rounded value and %st(1) the original
  buf[++offset] = '\xD9';
  buf[++offset] = '\xFC';

	acc << "fsub %st(0), %st(1)\n";
  buf[++offset] = '\xDC';
  buf[++offset] = '\xE9';

	acc << "fxch %st(1)\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\xC9';
	//st(1) holds integer part, st(0) fractional part


	acc << "f2xm1\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\xF0';
  //Performs 2^fracval - 1 and stores it in %st(0)

	//Add 1 to %st(0)
	acc << "fld1\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\xE8';

	acc << "faddp %st(0), %st(1)\n";
  buf[++offset] = '\xDE';
  buf[++offset] = '\xC1';

	//Perform %st(0) * 2^%st(1) and stores in %st(0)
	acc << "fscale\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\xFD';

	acc << "fxch %st(1)\n";
  buf[++offset] = '\xD9';
  buf[++offset] = '\xC9';

	acc << "fstp %st(0)\n";
  buf[++offset] = '\xDD';
  buf[++offset] = '\xD8';
  //Pops the fpu stack and throws away the result

  ++offset;
  return acc;
}

unsigned int exponentiation_operator_node::code_size() const {
  return leftChild->code_size() + rightChild->code_size() + 22;
}
