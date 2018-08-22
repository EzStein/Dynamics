#ifndef COMPILER_DATA_GUARD
#define COMPILER_DATA_GUARD
/*Contains data used during compilation*/
struct compiler_data {
  compiler_data(int _stackSizeFPU) : stackSizeFPU(_stackSizeFPU) { }
  /*Indicates the number of registers required to hold all floating point data at this point in the execution.
  The FPU has eight registers. When stackSizeFPU is greater than eight, special actions must be taken
  to load or store a floating point value from or to memory. They will be stored on the stack pointed to
  by the %rsp register*/
  int stackSizeFPU;
};
#endif
