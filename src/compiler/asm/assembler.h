#ifndef ASSEMBLER_H
#define ASSEMBLER_H

namespace asm {
  /*Assemblers the instruction described by the appropriate imformation.
  * Note that when the addressing mode is REG, immOffset is ignored and if
  the addressing mode is IMM, reg is ignored. If the addressing mode is memory,
  immOffset is added to the value stored in reg to get the address*/
  void append_amd64(amd64_instruction inst, amd64_operand_size opSize,
    amd64_addressing_mode modeSrc, amd64_register regSrc, long immOffsetSrc,
    amd64_addressing_mode modeDest, amd64_register regDest, long immOffsetDest,
    vector<unsigned char>& buf);
};

#endif
