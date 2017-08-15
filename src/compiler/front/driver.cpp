#include "compiler/front/driver.h"
#include "compiler/ast/AST.h"
#include "compiler/parser.h"
#include <string>
#include <sstream>
#include <iostream>
#include <list>

using std::stringstream;
using std::string;
using std::map;
using std::list;

driver::driver() { }

driver::~driver() {
  flush_all_buffers();
}

driver::driver(driver&& dr) : buffer_table(dr.buffer_table) {
  dr.buffer_table.clear();
}

driver& driver::operator=(driver&& dr) {
  if(&dr == this) return *this;

  /*Delete all previously allocated buffers*/
  flush_all_buffers();
  buffer_table = dr.buffer_table;
  dr.buffer_table.clear();

  return *this;
}


/*This function compiles the program. Since it is likely that many functions will be compiled
and free'd often, this function attempts to reuse allocated memory. If a memory area is allocated
and then marked as unused, it may be used for the new compilation instead of reallocating*/
template<class FUNC_TYPE> FUNC_TYPE driver::compile_as_function(string str) {
  stringstream sstream(str);
  parser parse(sstream);

  list<symbol> symbolTable;
  /*Compile the function*/
  AST ast(parse.parse(symbolTable));


  unsigned char * buf = nullptr;
  size_t size = ast.code_size();
  iter_t end = buffer_table.end();


  for(iter_t it = buffer_table.begin(); it != end; ++it) {
    if(it->second.available && it->second.size >= size) {
      /*We will use this preallocated memory for our function*/
      buf = reinterpret_cast<unsigned char *>(it->first);
      it->second.available = false;
      break;
    }
  }
  if(!buf) {
    /*allocate executable memory of the appropriate size*/
    buf = reinterpret_cast<unsigned char *>(mmap(NULL, size,
      PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    buffer_table[reinterpret_cast<void*>(buf)] = buffer_attributes(size, false);
  }

  ast.emit_code(std::cout, buf);
  FUNC_TYPE func = nullptr;
  /*A hack to allow us to set the function pointer to point to the buffer*/
  *reinterpret_cast<void**>(&func) = buf;

  return func;
}

template driver::var2_double_func_t driver::compile_as_function<driver::var2_double_func_t>(string);
template driver::var3_double_func_t driver::compile_as_function<driver::var3_double_func_t>(string);
template driver::var4_double_func_t driver::compile_as_function<driver::var4_double_func_t>(string);

template<class FUNC_TYPE>
void driver::mark_available(FUNC_TYPE func) {
  buffer_table.at(reinterpret_cast<void*>(func)).available = true;
}

template void driver::mark_available<driver::var2_double_func_t>(driver::var2_double_func_t);
template void driver::mark_available<driver::var3_double_func_t>(driver::var3_double_func_t);
template void driver::mark_available<driver::var4_double_func_t>(driver::var4_double_func_t);

/*
* Unmaps all buffers
*/
void driver::flush_all_buffers() {
  iter_t end = buffer_table.end();
  for(iter_t it = buffer_table.begin(); it != end; ++it) {
    munmap(it->first, it->second.size);
  }
  /*Removes all entries from the table after deallocating the buffers*/
  buffer_table.clear();
}

void driver::flush_unused_buffers() {

  iter_t end = buffer_table.end();
  iter_t it = buffer_table.begin();
  while(it != end) {
    if(it->second.available) {
      munmap(it->first, it->second.size);

      /*(Only the erased iterator is invalidated)*/
      iter_t tmp = it;
      ++it;
      buffer_table.erase(tmp);
    } else {
      ++it;
    }
  }
}
