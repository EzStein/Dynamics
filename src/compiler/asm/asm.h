/*
* Defines an enum classes that identifies all operators for
* the AMD64 instruction set
*/

#ifndef ASM_H
#define ASM_H
#include <string>



/*
* Lists supported intructions. Note that
* the operand size is specified separately.
* Note that not all instructions need reg's/addr mode
* so there is a NONE entry if none are needed.
*/
enum class instruction {
  MOV,
  PUSH, POP, STMXCSR, LDMXCSR, RETN, RETF,
  STD_ARITH_INSTRUCTION_START,
  ADD, OR, ADC, SBB, AND, SUB, XOR, CMP,
  STD_ARITH_INTSRUCTION_END,

  FPU_INSTRUCTION_START,
  FPU_STD_ARITH_INSTRUCTION_START,
  FADD, FADDP, FIADD, FMUL, FMULP, FIMUL, FSUB, FSUBP, FISUB, FSUBR, FSUBRP, FISUBR,
  FDIV, FDIVP, FIDIV, FDIVR, FDIVRP, FIDIVR,
  FPU_STD_ARITH_INSTRUCTION_END,

  FPU_NO_OPERAND_ARITH_INSTRUCTION_START,
  FNOP, FCHS, FABS, FTST, FXAM, FLD1,FLDL2T, FLDL2E, FLDPI,
  FLDLG2, FLDLN2, FLDZ, F2XM1, FY2LX, FPTAN, FPATAN, FXTRACT, FPREM1,
  FDECSTP, FINCSTP, FPREM, FY2LXP1, FSQRT, FSINCOS, FRNDINT,
  FSCALE, FSIN, FCOS, FUCOMPP, FNCLEX, FNINIT, FNSTSW,
  FPU_NO_OPERAND_ARITH_INSTRUCTION_END,
  FSTCW, FLDCW,
  FPU_MEM_INSTRUCTION_START,
  FST, FSTP, FIST, FISTP,FLD, FILD,
  FPU_MEM_INSTRUCTION_END,
  FPU_INSTRUCTION_END,
  NO_SUCH_INSTRUCTION

};

/*
* List of registers.
* The actual register used depends on the instruction, operand size and address size.
* R0 - AL AX EAX RAX ST0 MM0 XMM0
* R1 - CL CX ECX RCX ST1 MM1 XMM1
* R2 - DL DX EDX RDX ST2 MM2 XMM2
* R3 - BL BX EBX RBX ST3 MM3 XMM3
* R4 - (AH, SPL) SP ESP RSP ST4 MM4 XMM4
* R5 - (CH, BPL) BP EBP RBP ST5 MM5 XMM5
* R6 - (DH, SIL) SI ESI RSI ST6 MM6 XMM6
* R7 - (BH, DIL) DI EDI RDI ST7 MM7 XMM7
* R8 - R8L R8W R8D R8 XMM8
* R9 - R9L R9W R9D R9 XMM9
* R10 - R10L R10W R10D R10 XMM10
* R11 - R11L R11W R11D R11 XMM11
* R12 - R12L R12W R12D R12 XMM12
* R13 - R13L R13W R13D R13 XMM13
* R14 - R14L R14W R14D R14 XMM14
* R15 - R15L R15W R15D R15 XMM15
*/
enum class reg {
  R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
  NONE
};

enum class reg_type {
  GPR, FPU, MMX, XMM
};

/*
* List of operand sizes
* QUAD_WORD is 8 bytes,
* DOUBLE_WORD is 4,
* WORD is 2,
* BYTE is 1
*
* This is used to specify the operand size and/or address size of the instruction
*/
enum class size {
  TEN_BYTE,
  QUAD_WORD,
  DOUBLE_WORD,
  WORD,
  BYTE,
  NONE,
  NO_SUCH_SIZE,
  INVALID
};

enum class addressing_mode {
  REG,
  MEM,
  NONE
};

void str_to_reg(std::string str, reg& regRet, reg_type& regTypeRet, size& sizeRet, bool& uniform);
size char_to_size(char quant, bool fpu, bool fpuInt);
bool is_fpu_load_instruction(instruction inst);
bool is_fpu_pop_instruction(instruction inst);
bool is_fpu_integer_instruction(instruction inst);
bool is_fpu_instruction(instruction inst);
instruction str_to_instruction(std::string str);

#endif
