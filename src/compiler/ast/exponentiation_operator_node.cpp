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

std::ostream& exponentiation_operator_node::emit_code(std::ostream& acc, compiler_data& data) const {

  rightChild->emit_code(acc, data); //Now on %st(1) EXPONENT
  leftChild->emit_code(acc, data);  //Put on %st(0) BASE
  acc << "fyl2x\n";
  //Performs exponent * log2 (base) stores in %st(0)
  //We must now perform 2^%st(0)
	//We do this by performing 2^((int)%st0 + (frac)%st0)
	//Or 2^intval * 2^fracval
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xF1';

	acc << "fld %st(0)\n";	//Copies the value, Now it is held in %st(0) and %st(1)
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xC0';

	acc << "frndint\n";		//%st(0) now holds the rounded value and %st(1) the original
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xFC';

	acc << "fsub %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDC';
  data.executableBuf[++data.offset] = '\xE9';

	acc << "fxch %st(1)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xC9';
	//st(1) holds integer part, st(0) fractional part


	acc << "f2xm1\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xF0';
  //Performs 2^fracval - 1 and stores it in %st(0)

	//Add 1 to %st(0)
	acc << "fld1\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xE8';

	acc << "faddp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xC1';

	//Perform %st(0) * 2^%st(1) and stores in %st(0)
	acc << "fscale\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xFD';

	acc << "fxch %st(1)\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xC9';

	acc << "fstp %st(0)\n";
  data.executableBuf[++data.offset] = '\xDD';
  data.executableBuf[++data.offset] = '\xD8';
  //Pops the fpu stack and throws away the result

  return acc;
}

unsigned int exponentiation_operator_node::code_size() const {
  return leftChild->code_size() + rightChild->code_size() + 22;
}
