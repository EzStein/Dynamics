#include <cmath>
#include "exponentiation_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/visitor/mutation/pre_canonical_exponentiation_operator_visitor.h"

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

std::ostream& exponentiation_operator_node::emit_code_ia32(std::ostream& acc, compiler_data& data) const {

  rightChild->emit_code_ia32(acc, data); //Now on %st(1) EXPONENT
  leftChild->emit_code_ia32(acc, data);  //Put on %st(0) BASE
  acc << "fyl2x\n";
  //Performs exponent * log2 (base) stores in %st(0)
  //We must now perform 2^%st(0)
	//We do this by performing 2^((int)%st0 + (frac)%st0)
	//Or 2^intval * 2^fracval
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xF1';

  if(data.stackSizeFPU >= 9) {
    acc << "fldt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x2C';
    data.executableBuf[++data.offset] = '\x24';

    acc << "addl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xC4';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fincstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF7';
  }
  --data.stackSizeFPU;

  if(data.stackSizeFPU >= 8) {
    acc << "fdecstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF6';

    /*Make room for the extended double precision data on the stack*/
    acc << "subl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xEC';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fstpt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x3C';
    data.executableBuf[++data.offset] = '\x24';
  }

  /*Increment the size of the stack to reflected the added value*/
  ++data.stackSizeFPU;

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

  if(data.stackSizeFPU >= 8) {
    acc << "fdecstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF6';

    /*Make room for the extended double precision data on the stack*/
    acc << "subl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xEC';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fstpt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x3C';
    data.executableBuf[++data.offset] = '\x24';
  }

  /*Increment the size of the stack to reflected the added value*/
  ++data.stackSizeFPU;
	//Add 1 to %st(0)
	acc << "fld1\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xE8';

	acc << "faddp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xC1';

  if(data.stackSizeFPU >= 9) {
    acc << "fldt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x2C';
    data.executableBuf[++data.offset] = '\x24';

    acc << "addl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xC4';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fincstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF7';
  }
  --data.stackSizeFPU;

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
  if(data.stackSizeFPU >= 9) {
    acc << "fldt (%esp)\n";
    data.executableBuf[++data.offset] = '\xDB';
    data.executableBuf[++data.offset] = '\x2C';
    data.executableBuf[++data.offset] = '\x24';

    acc << "addl $10, %esp\n";
    data.executableBuf[++data.offset] = '\x81';
    data.executableBuf[++data.offset] = '\xC4';
    data.executableBuf[++data.offset] = '\x0A';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';
    data.executableBuf[++data.offset] = '\x00';

    acc << "fincstp\n";
    data.executableBuf[++data.offset] = '\xD9';
    data.executableBuf[++data.offset] = '\xF7';
  }
  --data.stackSizeFPU;

  return acc;
}

std::ostream& exponentiation_operator_node::emit_code_amd64(std::ostream& acc, compiler_data& data) const {

  rightChild->emit_code_amd64(acc, data); //Now on %st(1) EXPONENT
  leftChild->emit_code_amd64(acc, data);  //Put on %st(0) BASE
  acc << "fyl2x\n";
  //Performs exponent * log2 (base) stores in %st(0)
  //We must now perform 2^%st(0)
	//We do this by performing 2^((int)%st0 + (frac)%st0)
	//Or 2^intval * 2^fracval
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xF1';

  AST::emit_stack_dec_amd64(acc, data);

  AST::emit_stack_inc_amd64(acc, data);

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

  AST::emit_stack_inc_amd64(acc, data);
	//Add 1 to %st(0)
	acc << "fld1\n";
  data.executableBuf[++data.offset] = '\xD9';
  data.executableBuf[++data.offset] = '\xE8';

	acc << "faddp %st(0), %st(1)\n";
  data.executableBuf[++data.offset] = '\xDE';
  data.executableBuf[++data.offset] = '\xC1';

  AST::emit_stack_dec_amd64(acc, data);

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
  AST::emit_stack_dec_amd64(acc, data);

  return acc;
}

unsigned int exponentiation_operator_node::code_size() const {
  return leftChild->code_size() + rightChild->code_size() + 77;
}

expression_node* exponentiation_operator_node::copy() const {
  return AST::make_binary_operator_node<exponentiation_operator_node>(leftChild->copy(), rightChild->copy());
}

bool exponentiation_operator_node::is_integral() const {
  return leftChild->is_integral() && rightChild->is_integral();
}

void exponentiation_operator_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* exponentiation_operator_node::make_pre_canonical() {
  expression_node* newLeftChild = leftChild->make_pre_canonical();
  expression_node* newRightChild = rightChild->make_pre_canonical();
  if(newLeftChild != leftChild)
    delete leftChild;
  leftChild = newLeftChild;
  if(newRightChild != rightChild)
    delete rightChild;
  rightChild = newRightChild;

  expression_node* result;
  pre_canonical_exponentiation_operator_visitor visit(result, this);
  leftChild->accept(&visit);
  if(result != this) {
    expression_node* newResult = result->make_pre_canonical();
    if(newResult != result)
      delete result;
    result = newResult;
  }

  return result;
}

expression_node* exponentiation_operator_node::collect_terms() {
  expression_node* tmp = leftChild->collect_terms();
  if(tmp != leftChild)
    delete leftChild;
  leftChild = tmp;

  tmp = rightChild->collect_terms();
  if(tmp != rightChild)
    delete rightChild;
  rightChild = tmp;

  return this;
}

expression_node* exponentiation_operator_node::optimization_round() {
  binary_operator_node::optimization_round();
  if(evaluatable()) {
    if(is_integral()) {
      return new integer_number_leaf_node(evaluate());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  if(leftChild->evaluatable() && leftChild->evaluate() == 1) {
    return new integer_number_leaf_node(1);
  } else if(rightChild->evaluatable()) {
    if(rightChild->evaluate() == 1) {
      expression_node* tmp = leftChild;
      leftChild = nullptr;
      return tmp;
    } else if(rightChild->evaluate() == 0) {
      if(leftChild->evaluatable() && leftChild->evaluate() == 0)
        return this;
      else
        return new integer_number_leaf_node(1);
    } else {
      return this;
    }
  } else {
    return this;
  }
}

expression_node* exponentiation_operator_node::differentiate(const std::string& var) {
  expression_node* leftDerivative = leftChild->copy();
  expression_node* tmp = leftDerivative->differentiate(var);
  if(tmp != leftDerivative)
    delete leftDerivative;
  leftDerivative = tmp;
  expression_node* rightDerivative = rightChild->copy();
  tmp = rightDerivative->differentiate(var);
  if(tmp != rightDerivative)
    delete rightDerivative;
  rightDerivative = tmp;

  expression_node* retVal =
    new polyadic_multiplication_operator_node(
    new exponentiation_operator_node(leftChild->copy(), rightChild->copy()),
    new polyadic_addition_operator_node(
    new polyadic_multiplication_operator_node(
    rightChild->copy(), new exponentiation_operator_node(leftChild->copy(),
    new integer_number_leaf_node(-1)), leftDerivative), new polyadic_multiplication_operator_node(
    rightDerivative, /*THIS OUGHT TO BE NATURAL LOGARITHM*/leftChild->copy()
    )));
  return retVal;
}
