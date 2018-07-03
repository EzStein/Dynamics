#include <iomanip>
#include <sstream>
#include "compiler/ast/number_leaf_node.h"
#include "compiler/ast/AST.h"


number_leaf_node::number_leaf_node(double _val) : val(_val) {

}

double number_leaf_node::evaluate() const {
  return val;
}

long number_leaf_node::evaluate_as_integer() const {
  return static_cast<long>(val);
}


std::ostream& number_leaf_node::print(std::ostream& out) const {
  out << val;
  return out;
}

bool number_leaf_node::evaluatable() const {
  return true;
}

void number_leaf_node::emit_code_amd64(std::string& acc, compiler_data& data) const {
  AST::emit_stack_inc_amd64(acc, data);

  std::stringstream sstream;
  const unsigned char * ptr = reinterpret_cast<const unsigned char*>(&val);
  sstream << std::hex;
  sstream << "movl $0x";
  for(int i = 7; i >= 4; --i) {
    const unsigned int toWrite = static_cast<const unsigned int>(ptr[i]);
    if(toWrite <= 0x0f)
      sstream << "0";
    sstream << toWrite;
  }
  sstream << ", %eax\n";
  acc += sstream.str();
  acc += "movl %eax, -0x0c(%rbp)\n";

  sstream = std::stringstream();
  sstream << std::hex;
  sstream << "movl $0x";
  for(int i = 3; i >= 0; --i) {
    const unsigned int toWrite = static_cast<const unsigned int>(ptr[i]);
    if(toWrite <= 0x0f)
      sstream << "0";
    sstream << toWrite;
  }
  sstream << ", %eax\n";
  acc += sstream.str();
  acc += "movl %eax, -0x10(%rbp)\n";

  acc += "fldl -0x10(%rbp)\n";
}

expression_node* number_leaf_node::copy() const {
  return AST::make_number_leaf_node(val);
}

bool number_leaf_node::is_integral() const {
  return false;
}

void number_leaf_node::accept(visitor* v) {
  v->visit(this);
}

expression_node* number_leaf_node::differentiate(const std::string& var) {
  return new integer_number_leaf_node(0);
}
