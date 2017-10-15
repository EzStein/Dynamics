#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "compiler/asm/asm.h"
#include <vector>
class assembler {
public:
  /*Assemblers the instruction described by the appropriate imformation.
  * Note that when the addressing mode is REG, rmDisp is ignored. If the addressing mode is memory,
  offset is added to the value stored in rmReg to get the address. Currently
  SIB is not supported although it is used to address displacement of ESP/RSP.
   If isImm is true then rReg, rmIsDest are ignored. Otherwise imm is ignored.
   * 
   * If rmIsDest is true then the modrm addressing mode is the destination, otherwise
   * it is the source. The uniformReg bool indicates whether we are using 
   * a uniform byte register BPL SPL DIL SIL instead of AH BH CH DH.
   * If so, we ensure that there if this register is actually used, 
   * then we encode an REX byte and do not encode one otherwise.
   */
  void append(instruction inst, size operandSize, size addressSize,
  addressing_mode addrMode, reg rmReg, long rmDisp, 
  reg rReg, bool isImm, long imm, bool rmIsDest, bool uniformReg,
  std::vector<unsigned char>& buf);
  std::vector<unsigned char> assemble(std::string str);
};

#endif
