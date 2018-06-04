#ifndef DYNSOLVER_COMPILER_FUNCTION_H_
#define DYNSOLVER_COMPILER_FUNCTION_H_

#include <algorithm>
#include <cstring>
#include <vector>

#include <sys/mman.h>

namespace dynsolver {
namespace compiler {

// This class represents a compiled function which may be called.
// The class wraps the memory of the compiled function so that it
// can manage the memory internally.
// This class is templated so as to allow any function signature.
template<class RET_T, class... ARGS_T>
class function {
 private:
  typedef RET_T (*FUNC_PTR_T)(ARGS_T...);
  unsigned char* funcPtr;
  size_t size;

 public:
  // Constructs a function from the bytecode provided. This is done by
  // allocating executable memory and writing the bytecode into that memory.
  function(const std::vector<unsigned char>&);

  // Destructor
  ~function();

  // Copy and move constructor
  function(const function<RET_T, ARGS_T...>&);
  function(function<RET_T, ARGS_T...>&&);

  // Copy and move assignment
  function<RET_T, ARGS_T...>& operator=(const function<RET_T, ARGS_T...>&);
  function<RET_T, ARGS_T...>& operator=(function<RET_T, ARGS_T...>&&);

  // Calls the function
  RET_T operator()(ARGS_T...) const;
};

template<class RET_T, class ...ARGS_T>
function<RET_T, ARGS_T...>::function(const std::vector<unsigned char>& code) :
    size(0), funcPtr(nullptr) {
  size = code.size();
  // Allocates readable writable executable memory.
  funcPtr = reinterpret_cast<unsigned char *>(
      mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE,
           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
  std::copy(code.begin(), code.end(), funcPtr);
}

template<class RET_T, class ...ARGS_T>
function<RET_T, ARGS_T...>::~function() {
  // We dealocate the speciall executable memory.
  if(funcPtr != nullptr)
    munmap(reinterpret_cast<void*>(funcPtr), size);
}

template<class RET_T, class ...ARGS_T> function<RET_T, ARGS_T...>
::function(const function<RET_T, ARGS_T...>& other) :
    size(0), funcPtr(nullptr) {
  size = other.size;
  funcPtr = reinterpret_cast<unsigned char *>(
      mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE,
           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
  std::memcpy(funcPtr, other.funcPtr, size);
}
template<class RET_T, class ...ARGS_T>
function<RET_T, ARGS_T...>::function(function<RET_T, ARGS_T...>&& other) :
    size(0), funcPtr(nullptr) {
  size = other.size;
  funcPtr = other.funcPtr;
  other.funcPtr = nullptr;
}

template<class RET_T, class ...ARGS_T> function<RET_T, ARGS_T...>&
function<RET_T, ARGS_T...>
::operator=(const function<RET_T, ARGS_T...>& other) {
  if(&other == this) return *this;
  munmap(reinterpret_cast<void*>(funcPtr), size);
  size = other.size;
  funcPtr = reinterpret_cast<unsigned char *>(
      mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE,
           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
  std::memcpy(funcPtr, other.funcPtr, size);
  return *this;
}


template<class RET_T, class ...ARGS_T> function<RET_T, ARGS_T...>&
function<RET_T, ARGS_T...>
::operator=(function<RET_T, ARGS_T...>&& other) {
  if(&other == this) return *this;
  munmap(reinterpret_cast<void*>(funcPtr), size);
  size = other.size;
  funcPtr = other.funcPtr;
  other.funcPtr = nullptr;
  return *this;
}

template<class RET_T, class ...ARGS_T>
RET_T function<RET_T, ARGS_T...>::operator()(ARGS_T... args) const {
  FUNC_PTR_T callableFuncPtr = nullptr;
  
  // A hack which allows us to point callableFuncPtr to the location
  // pointed to by funcPtr. A simple reinterpret cast would not compile.
  *reinterpret_cast<void**>(&callableFuncPtr) = funcPtr;
  return callableFuncPtr(args...);
}

} // namespace compiler
} // namespace dynsolver
#endif
