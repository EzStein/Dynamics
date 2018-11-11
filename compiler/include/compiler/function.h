#ifndef DYNSOLVER_COMPILER_FUNCTION_H_
#define DYNSOLVER_COMPILER_FUNCTION_H_

#include <algorithm>
#include <cstring>
#include <vector>
#include <cassert>

#ifdef IS_UNIX
#include <sys/mman.h>
#else
#include <windows.h>
#endif

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
  explicit function(const std::vector<unsigned char>&);

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
  funcPtr(nullptr), size(0) {
  size = code.size();
  // Allocates readable writable executable memory.
#ifdef IS_UNIX
  void* ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANON, -1, 0);
#else
  void* ptr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#endif
  funcPtr = reinterpret_cast<unsigned char *>(ptr);
  std::memcpy(funcPtr, code.data(), size * sizeof(unsigned char));
}

template<class RET_T, class ...ARGS_T>
function<RET_T, ARGS_T...>::~function() {
  // We dealocate the speciall executable memory.
  if(funcPtr != nullptr) {
#ifdef IS_UNIX
    int success = munmap(reinterpret_cast<void*>(funcPtr), size);
#else
    int success = !VirtualFree(reinterpret_cast<void*>(funcPtr), 0, MEM_RELEASE);
#endif
    assert(success == 0);
    funcPtr = nullptr;
  }
}

template<class RET_T, class ...ARGS_T> function<RET_T, ARGS_T...>
::function(const function<RET_T, ARGS_T...>& other) :
  funcPtr(nullptr), size(0) {
  size = other.size;

#ifdef IS_UNIX
  void* ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANON, -1, 0);
#else
  void* ptr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#endif
  
  funcPtr = reinterpret_cast<unsigned char *>(ptr);
  std::memcpy(funcPtr, other.funcPtr, size);
}

template<class RET_T, class ...ARGS_T>
function<RET_T, ARGS_T...>::function(function<RET_T, ARGS_T...>&& other) :
  funcPtr(nullptr), size(0) {
  size = other.size;
  funcPtr = other.funcPtr;
  other.funcPtr = nullptr;
}

template<class RET_T, class ...ARGS_T> function<RET_T, ARGS_T...>&
function<RET_T, ARGS_T...>
::operator=(const function<RET_T, ARGS_T...>& other) {
  if(&other == this) return *this;
#ifdef IS_UNIX
  int success = munmap(reinterpret_cast<void*>(funcPtr), size);
#else
  int success = !VirtualFree(reinterpret_cast<void*>(funcPtr), 0, MEM_RELEASE);
#endif
  assert(success == 0);
  size = other.size;
#ifdef IS_UNIX
  void* ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANON, -1, 0);
#else
  void* ptr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#endif
  funcPtr = reinterpret_cast<unsigned char *>(ptr);
  std::memcpy(funcPtr, other.funcPtr, size);
  return *this;
}


template<class RET_T, class ...ARGS_T> function<RET_T, ARGS_T...>&
function<RET_T, ARGS_T...>
::operator=(function<RET_T, ARGS_T...>&& other) {
  if(&other == this) return *this;
  int success = munmap(reinterpret_cast<void*>(funcPtr), size);
  assert(success == 0);
  size = other.size;
  funcPtr = other.funcPtr;
  other.funcPtr = nullptr;
  return *this;
}

template<class RET_T, class ...ARGS_T>
RET_T function<RET_T, ARGS_T...>::operator()(ARGS_T... args) const {
  FUNC_PTR_T callableFuncPtr = reinterpret_cast<FUNC_PTR_T>(funcPtr);
  
  // A hack which allows us to point callableFuncPtr to the location
  // pointed to by funcPtr. A simple reinterpret cast would not compile.
  // *reinterpret_cast<void**>(&callableFuncPtr) = funcPtr;
  return callableFuncPtr(args...);
}

} // namespace compiler
} // namespace dynsolver
#endif
