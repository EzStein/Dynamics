#include <cmath>
#include "compiler/ast/exponentiation_operator_node.h"
#include "compiler/ast/AST.h"
#include "compiler/ast/visitor/mutation/pre_canonical_exponentiation_operator_visitor.h"
#include "compiler/ast/integer_number_leaf_node.h"
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/polyadic_multiplication_operator_node.h"
#include "compiler/ast/polyadic_addition_operator_node.h"

exponentiation_operator_node::exponentiation_operator_node(expression_node* leftChild, expression_node* rightChild) :
binary_operator_node(leftChild, rightChild) {

}

double exponentiation_operator_node::evaluate() const {
  return pow(leftChild->evaluate(), rightChild->evaluate());
}

long exponentiation_operator_node::evaluate_as_integer() const {
  long base = leftChild->evaluate_as_integer();
  long exponent = rightChild->evaluate_as_integer();
  long acc = 1;
  if(exponent == 0 && base != 0)
    return 1;
  else if(exponent > 0) {
    for(; exponent != 0; exponent--) {
      acc *= base;
    }
    return acc;
  } else if(exponent < 0) {
    for(; exponent != 0; exponent--) {
      acc *= base;
    }
    return 1/acc;
  } else {
    throw "0^0 is not defined!";
  }
}


std::ostream& exponentiation_operator_node::print(std::ostream& out) const {
  out << '(';
  leftChild->print(out) << ')' << '^' << '(';
  rightChild->print(out) << ')';
  return out;
}

void exponentiation_operator_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  if(rightChild->is_integral()) {
    if(rightChild->evaluatable()) {
      long exponent = rightChild->evaluate_as_integer();
      if(exponent == 0) {
        /*we simply load one onto the stack*/
        AST::emit_stack_inc_amd64(acc, data);
        acc +=
        "fld1\n";
      } else if(exponent == 1) {
        /*The result is simply the left child*/
        leftChild->emit_code_amd64(acc, data);
      } else if(exponent == -1) {
        /*The result is 1 over the left child*/
        leftChild->emit_code_amd64(acc, data);
        AST::emit_stack_inc_amd64(acc, data);
        acc += "fld1\n";
        /*ST0 is 1 and ST1 is base*/
        acc += "fdivrp %st(0), %st(1)\n";
        AST::emit_stack_dec_amd64(acc, data);
      } else if(exponent > 1) {
        leftChild->emit_code_amd64(acc, data); /*Put base into %st(0)*/
        AST::emit_stack_inc_amd64(acc, data);
        acc +=
        "fld %st(0)\n"; /*Copy base to %st(1)*/
        /*ST0 and ST1 now both contain the base and exponent*/
        for(; exponent != 2; --exponent)
          acc += "fmul %st(0), %st(1)\n";
        acc += "fmulp %st(0), %st(1)\n";
        AST::emit_stack_dec_amd64(acc, data);
      } else {
        exponent = -1*exponent;
        leftChild->emit_code_amd64(acc, data); /*Put base into %st(0)*/
        AST::emit_stack_inc_amd64(acc, data);
        acc +=
        "fld %st(0)\n"; /*Copy base to %st(1)*/
        /*ST0 and ST1 now both contain the base and exponent*/
        for(; exponent != 2; --exponent)
          acc += "fmul %st(0), %st(1)\n";
        acc += "fmulp %st(0), %st(1)\n";
        AST::emit_stack_dec_amd64(acc, data);
        /*Now we must form 1/st(0)*/
        AST::emit_stack_inc_amd64(acc, data);
        acc += "fld1\n";
        /*ST0 is 1 and ST1 is base*/
        acc += "fdivrp %st(0), %st(1)\n";
        AST::emit_stack_dec_amd64(acc, data);
      }
    } else {
      throw "Non evauluatble expressions with integral exponent are not yet compilable!";
    }

    /*
    * We first store the exponent value in a GPR (%rax)
    */
    /*rightChild->emit_code_amd64(acc, data);
    acc +=
    "fistpq -0x50(%rbp)\n"; /*Store and pop the value as an integer*/
    /*AST::emit_stack_dec_amd64(acc, data);
    acc +=
    "movq -0x50(%rbp), %rax\n"
    "subq $1, %rax\n";

    /*Put the base into ST(0)*/
    /*leftChild->emit_code_amd64(acc, data);
    /*Copy the base from ST(0) to ST(1)*/
    /*AST::emit_stack_inc_amd64(acc, data);
    acc += "fld %st(0)\n";

    /*Multiply the two together and store it in %st(0), %st(1) is unchanged*/
    /*acc += "LABEL:\n"
    acc += "fmul %st(1), %st(0)\n";
    /*Subtract 1 from rax*/
    /*acc += "subq $1, %rax\n";
    acc += "cmp %rax\n";
    acc += "jne LABEL\n";*/


  } else {
    rightChild->emit_code_amd64(acc, data); //Now on %st(1) EXPONENT
    leftChild->emit_code_amd64(acc, data);  //Put on %st(0) BASE

    acc += "fyl2x\n";
    //Performs exponent * log2 (base) stores in %st(0)
    //We must now perform 2^%st(0)
  	//We do this by performing 2^((int)%st0 + (frac)%st0)
  	//Or 2^intval * 2^fracval
    AST::emit_stack_dec_amd64(acc, data);

    AST::emit_stack_inc_amd64(acc, data);

  	acc +=
    "fld %st(0)\n"	//Copies the value, Now it is held in %st(0) and %st(1)
    "frndint\n"		//%st(0) now holds the rounded value and %st(1) the original
  	"fsub %st(0), %st(1)\n"
    "fxch %st(1)\n"
  	//st(1) holds integer part, st(0) fractional part

    "f2xm1\n";
    //Performs 2^fracval - 1 and stores it in %st(0)


    AST::emit_stack_inc_amd64(acc, data);

  	//Add 1 to %st(0)
  	acc +=
    "fld1\n"
  	"faddp %st(0), %st(1)\n";
    AST::emit_stack_dec_amd64(acc, data);
  	//Perform %st(0) * 2^%st(1) and stores in %st(0)
  	acc +=
    "fscale\n"
    "fxch %st(1)\n"
    "fstpl %st(0)\n";
    //Stores st0 into itself then pops the stack
    AST::emit_stack_dec_amd64(acc, data);
  }
}

expression_node* exponentiation_operator_node::copy() const {
  return AST::make_binary_operator_node<exponentiation_operator_node>(leftChild->copy(), rightChild->copy());
}

bool exponentiation_operator_node::is_integral() const {
  return leftChild->is_integral() && rightChild->is_integral() &&
  (rightChild->evaluate_as_integer() >= 0);
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
      return new integer_number_leaf_node(evaluate_as_integer());
    } else {
      return new number_leaf_node(evaluate());
    }
  }
  if(leftChild->evaluatable() && leftChild->is_integral() && leftChild->evaluate_as_integer() == 1) {
    return new integer_number_leaf_node(1);
  } else if(rightChild->evaluatable() && rightChild->is_integral()) {
    if(rightChild->evaluate_as_integer() == 1) {
      expression_node* tmp = leftChild;
      leftChild = nullptr;
      return tmp;
    } else if(rightChild->evaluate_as_integer() == 0) {
      if(leftChild->evaluatable() && leftChild->is_integral() && leftChild->evaluate_as_integer() == 0)
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
