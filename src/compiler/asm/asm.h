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
  STD_ARITH_INSTRUCTION_START,
  ADD, OR, ADC, SBB, AND, SUB, XOR, CMP,
  STD_ARITH_INTSRUCTION_END,

  FPU_STD_ARITH_INSTRUCTION_START,
  FADD, FADDP, FIADD, FMUL, FMULP, FIMUL, FSUB, FSUBP, FISUB, FSUBR, FSUBRP, FISUBR,
  FDIV, FDIVP, FIDIV, FDIVR, FDIVRP, FIDIVR,
  FPU_STD_ARITH_INSTRUCTION_END,

  FPU_NO_OPERAND_ARITH_INSTRUCTION_START,
  FNOP, FCHS, FABS, FTST, FXAM, FLD1,FLDL2T, FLDL2E, FLDPI,
  FLDLG2, FLDLN2, FLDZ, F2XM1, FY2LX, FPTAN, FPATAN, FXTRACT, FPREM1,
  FDECSTP, FINCSTP, FPREM, FY2LXP1, FSQRT, FSINCOS, FRNDINT,
  FSCALE, FSIN, FCOS, FUCOMPP, FNCLEX, FNINIT,FNSTSW,
  FPU_NO_OPERAND_ARITH_INSTRUCTION_END,

  FPU_MEM_INSTRUCTION_START,
  FST, FSTP, FIST, FISTP,FLD, FILD,
  FPU_MEM_INSTRUCTION_END

};

instruction str_to_instruction(std::string str) {
  if(str == )
    return instruction::ADD;
  else if(str == )
    return instruction::OR;
  else if(str == )
    return instruction::ADC;
  else if(str == )
    return instruction::SBB;
  else if(str == )
    return instruction::AND;
  else if(str == )
    return instruction::SUB;
  else if(str == )
    return instruction::XOR;
  else if(str == )
    return instruction::CMP;
  else if(str == )
    return instruction::FADD;
  else if(str == )
    return instruction::FADDP;
  else if(str == )
    return instruction::FIADD;
  else if(str == )
    return instruction::FMUL;
  else if(str == )
    return instruction::FMULP;
  else if(str == )
    return instruction::FIMUL;
  else if(str == )
    return instruction::FSUB;
  else if(str == )
    return instruction::FSUBP;
  else if(str == )
    return instruction::FISUB;
  else if(str == )
    return instruction::FSUBR;
  else if(str == )
    return instruction::FSUBRP;
  else if(str == )
    return instruction::FISUBR;
  else if(str == )
    return instruction::FDIV;
  else if(str == )
    return instruction::FDIVP;
  else if(str == )
    return instruction::FIDIV;
  else if(str == )
    return instruction::FDIVR;
  else if(str == )
    return instruction::FDIVRP;
  else if(str == )
    return instruction::FIDIVR;
  else if(str == )
    return instruction::FNOP;
  else if(str == )
    return instruction::FCHS;
  else if(str == )
    return instruction::FABS;
  else if(str == )
    return instruction::FTST;
  else if(str == )
    return instruction::FXAM;
  else if(str == )
    return instruction::FLD1;
  else if(str == )
    return instruction::FLDL2T;
  else if(str == "FLDL2E")
    return instruction::FLDL2E;
  else if(str == "FLDPI")
    return instruction::FLDPI;
  else if(str == "FLDLG2")
    return instruction::FLDLG2;
  else if(str == "FLDLN2")
    return instruction::FLDLN2;
  else if(str == "FLDZ")
    return instruction::FLDZ;
  else if(str == "F2XM1")
    return instruction::F2XM1;
  else if(str == "FY2LX")
    return instruction::FY2LX;
  else if(str == "FPTAN")
    return instruction::FPTAN;
  else if(str == "FPATAN")
    return instruction::FPATAN;
  else if(str == "FXTRACT")
    return instruction::FXTRACT;
  else if(str == "FPREM1")
    return instruction::FPREM1;
  else if(str == "FDECSTP")
    return instruction::FDECSTP;
  else if(str == "FINCSTP")
    return instruction::FINCSTP;
  else if(str == "FPREM")
    return instruction::FPREM;
  else if(str == "FY2LXP1")
    return instruction::FY2LXP1;
  else if(str == "FSQRT")
    return instruction::FSQRT;
  else if(str == "FSINCOS")
    return instruction::FSINCOS;
  else if(str == "FRNDINT")
    return instruction::FRNDINT;
  else if(str == "FSCALE")
    return instruction::FSCALE;
  else if(str == "FSIN")
    return instruction::FSIN;
  else if(str == "FCOS")
    return instruction::FCOS;
  else if(str == "FUCOMPP")
    return instruction::FUCOMPP;
  else if(str == "FNCLEX")
    return instruction::FNCLEX;
  else if(str == "FNINIT")
    return instruction::FNINIT;
  else if(str == "FNSTSW")
    return instruction::FNSTSW;
  else if(str == "FST")
    return instruction::FST;
  else if(str == "FSTP")
    return instruction::FSTP;
  else if(str == "FIST")
    return instruction::FIST;
  else if(str == "FISTP")
    return instruction::FISTP;
  else if(str == "FLD")
    return instruction::FLD;
  else if(str == "FILD")
    return instruction::FILD;
}

bool is_fpu_integer_instruction(instruction inst) {
  switch(inst) {
    case instruction::FIADD:
    case instruction::FISUB:
    case instruction::FISUBR:
    case instruction::FIDIV:
    case instruction::FIDIVR:
    case instruction::FIMUL:
    case instruction::FIST:
    case instruction::FISTP:
    case instruction::FILD:
       return true;
     default: return false;
   };
}

bool is_fpu_pop_instruction(instruction inst) {
  switch(inst) {
    case instruction::FADDP:
    case instruction::FSUBP:
    case instruction::FSUBRP:
    case instruction::FDIVP:
    case instruction::FDIVRP:
    case instruction::FMULP:
    case instruction::FSTP:
    case instruction::FISTP:
       return true;
     default: return false;
   }
}
bool is_fpu_load_instruction(instruction inst) {
  switch(inst) {
    case instruction::FILD:
    case instruction::FLD:
      return true;
    default: return false;
  }
}

/*
* List of registers.
* The actual register used depends on the instruction, operand size and address size.
* R0 - AL AX EAX RAX ST0
* R1 - CL CX ECX RCX ST1
* R2 - DL DX EDX RDX ST2
* R3 - BL BX EBX RBX ST3
* R4 - (AH, SPL) SP ESP RSP ST4
* R5 - (CH, BPL) BP EBP RBP ST5
* R6 - (DH, SIL) SI ESI RSI ST6
* R7 - (BH, DIL) DI EDI RDI ST7
* R8 - R8L R8W R8D R8
* R9 - R9L R9W R9D R9
* R10 - R10L R10W R10D R10
* R11 - R11L R11W R11D R11
* R12 - R12L R12W R12D R12
* R13 - R13L R13W R13D R13
* R14 - R14L R14W R14D R14
* R15 - R15L R15W R15D R15
*/
enum class reg {
  R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
  NONE
};

enum class addressing_mode {
  REG,
  MEM,
  NONE
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
  NONE
};

#endif
