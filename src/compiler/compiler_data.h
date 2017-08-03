#ifndef COMPILER_DATA_GUARD
#define COMPILER_DATA_GUARD
/*Contains data used during compilation*/
struct compiler_data {
  /*The location in the executableBuf of the last machine code byte that was placed*/
  int offset;

  /*Indicates the number of registers required to hold all floating point data at this point in the execution.
  The FPU has eight registers. When stackSizeFPU is greater than eight, special actions must be taken
  to load or store a floating point value from or to memory. They will be stored on the stack pointed to
  by the %esp register*/
  int stackSizeFPU;

  /*A pointer to an exectable area of memory where the machine code will be written to*/
  unsigned char * executableBuf;
};
#endif
