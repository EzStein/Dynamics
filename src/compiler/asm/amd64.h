/*
* Defines an enum classes that identifies all operators
*/

#ifndef AMD64_H
#define AMD64_H
/*
* Lists supported intructions. Note that
* the operand size is specified separately
*/
enum class amd64_instruction {
  mov, push, sub, add,
};

/*
* List of registers.
*/
enum class amd64_register {
  AL, AH, AX, EAX, RAX,
  BL, BH, BX, EBX, RBX,
  CL, CH, CX, ECX, RCX,
  DL, DH, DX, EDX, RDX,
  SI, ESI, RSI,
  DI, EDI, RDI,
  BP, EBP, RBP,
  SP, ESP, RSP,
  XMM0,  XMM1,  XMM2,  XMM3,  XMM4,  XMM5,  XMM6,  XMM7,
  ST0,  ST1,  ST2,  ST3,  ST4,  ST5,  ST6,  ST7,
  R8, R9, R10, R11, R12, R13, R14, R15,
  R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D,
  R8W, R9W, R10W, R11W, R12W, R13W, R14W, R15W,
  R8L, R9L, R10L, R11L, R12L, R13L, R14L, R15L
}

enum class amd64_addressing_mode {
  IMM,
  REG,
  MEM
}

/*
* List of operand sizes
* QUAD is 8 bytes,
* LONG is 4,
* WORD is 2,
* BYTE is 1
*/
enum class amd64_operand_size {
  QUAD,
  LONG,
  WORD,
  BYTE
};

#endif
