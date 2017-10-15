#include "compiler/asm/asm.h"
#include <string>
#include <iostream>
#include <cctype>
#include <algorithm>
instruction str_to_instruction(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  if(str == "ADD")
    return instruction::ADD;
  else if(str == "OR")
    return instruction::OR;
  else if(str == "ADC")
    return instruction::ADC;
  else if(str == "SBB")
    return instruction::SBB;
  else if(str == "AND")
    return instruction::AND;
  else if(str == "SUB")
    return instruction::SUB;
  else if(str == "XOR")
    return instruction::XOR;
  else if(str == "CMP")
    return instruction::CMP;
  else if(str == "FADD")
    return instruction::FADD;
  else if(str == "FADDP")
    return instruction::FADDP;
  else if(str == "FIADD")
    return instruction::FIADD;
  else if(str == "FMUL")
    return instruction::FMUL;
  else if(str == "FMULP")
    return instruction::FMULP;
  else if(str == "FIMUL")
    return instruction::FIMUL;
  else if(str == "FSUB")
    return instruction::FSUB;
  else if(str == "FSUBP")
    return instruction::FSUBP;
  else if(str == "FISUB")
    return instruction::FISUB;
  else if(str == "FSUBR")
    return instruction::FSUBR;
  else if(str == "FSUBRP")
    return instruction::FSUBRP;
  else if(str == "FISUBR")
    return instruction::FISUBR;
  else if(str == "FDIV")
    return instruction::FDIV;
  else if(str == "FDIVP")
    return instruction::FDIVP;
  else if(str == "FIDIV")
    return instruction::FIDIV;
  else if(str == "FDIVR")
    return instruction::FDIVR;
  else if(str == "FDIVRP")
    return instruction::FDIVRP;
  else if(str == "FIDIVR")
    return instruction::FIDIVR;
  else if(str == "FNOP")
    return instruction::FNOP;
  else if(str == "FCHS")
    return instruction::FCHS;
  else if(str == "FABS")
    return instruction::FABS;
  else if(str == "FTST")
    return instruction::FTST;
  else if(str == "FXAM")
    return instruction::FXAM;
  else if(str == "FLD1")
    return instruction::FLD1;
  else if(str == "FLDL2T")
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
  else if(str == "MOV")
    return instruction::MOV;
  else if(str == "PUSH")
    return instruction::PUSH;
  else if(str == "POP")
    return instruction::POP;
  else if(str == "STMXCSR")
    return instruction::STMXCSR;
  else if(str == "FSTCW")
    return instruction::FSTCW;
  else if(str == "FLDCW")
    return instruction::FLDCW;
  else if(str == "RETN" || str == "RET")
    return instruction::RETN;
  else if(str == "RETF")
    return instruction::RETF;
  else if(str == "LDMXCSR")
    return instruction::LDMXCSR;
  else
    return instruction::NO_SUCH_INSTRUCTION;
}

bool is_fpu_instruction(instruction inst) {
  return inst > instruction::FPU_INSTRUCTION_START && inst < instruction::FPU_INSTRUCTION_END;
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
* Assumes the input string is in upper case
 * Returns the register associated with this string along with it's operand
 * size. For GPR registers, the size is also returned.
 * A bool is also returned indicating whether it is a uniform reg.
 */
void str_to_reg(std::string str, reg& regRet, reg_type& regTypeRet, size& sizeRet, bool& uniform) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  /*If the first character after the % is an r, and the next one is a digit*/
  if(str[1] == 'R' && isdigit(str[2])) {
    /*We must be using one of the extended GPR registers*/
    regTypeRet = reg_type::GPR;
    uniform = false;
    int reg_num = 0;
    if(str.size() >= 4 && isdigit(str[3])) {
      reg_num = std::stoi(std::string(1, str[2]) + std::string(1,str[3]));
    } else {
      reg_num = std::stoi(std::string(1,str[2]));
    }
    std::cout << "!!!: " << reg_num <<std::endl;
    regRet = static_cast<reg>(reg_num);
    char q = *(str.end()-1);
    if(q == 'L') {
      sizeRet = size::BYTE;
    } else if(q == 'W') {
      sizeRet = size::WORD;
    } else if(q == 'D') {
      sizeRet = size::DOUBLE_WORD;
    } else {
      sizeRet = size::QUAD_WORD;
    }
  } else if(str.size() >= 3 && str[1] == 'S' && str[2] == 'T') {
    /*This is an FPU reg*/
    regTypeRet = reg_type::FPU;
    uniform = false;
    sizeRet = size::NONE;
    int reg_num = std::stoi(std::string(1,*(str.end()-2)));
    
    regRet = static_cast<reg>(reg_num);
  } else if(str.size() >= 3 && str[1] == 'M' && str[2] == 'M') {
    /*This is a MMX reg*/
    regTypeRet = reg_type::MMX;
    uniform = false;
    sizeRet = size::NONE;
    int reg_num = std::stoi(std::string(1,*(str.end()-1)));
    regRet = static_cast<reg>(reg_num);
  } else if(str.size() >= 4 && str[1] == 'X' && str[2] == 'M' && str[3] == 'M') {
    /*We have an XMM reg*/
    regTypeRet = reg_type::XMM;
    uniform = false;
    sizeRet = size::NONE;
    if(isdigit(*(str.end()-2))) {
      regRet = static_cast<reg>(std::stoi(std::string(1,*(str.end()-2)) + std::string(1,*(str.end()-1))));
    } else {
      regRet = static_cast<reg>(std::stoi(std::string(1,*(str.end()-1))));
    }
  } else {
    /*We have a GPR*/
    regTypeRet = reg_type::GPR;
    if(str[1] == 'R' || str[1] == 'E') {
      std::cout << str <<std::endl;
      uniform = false;
      std::string val = std::string(1,str[2]) + std::string(1,str[3]);
      if(val == "AX") {
        regRet = reg::R0;
      } else if(val == "CX") {
        regRet = reg::R1;
      } else if(val == "DX") {
        regRet = reg::R2;
      } else if(val == "BX") {
        regRet = reg::R3;
      } else if(val == "SP") {
        regRet = reg::R4;
      } else if(val == "BP") {
        regRet = reg::R5;
      } else if(val == "SI") {
        regRet = reg::R6;
      } else if(val == "DI") {
        regRet = reg::R7;
      }

      if(str[1] == 'R') {
        sizeRet = size::QUAD_WORD;
      } else if(str[1] == 'E') {
        sizeRet = size::DOUBLE_WORD;
      }
    } else {
      if(str.size() == 3) {
        uniform = false;
        std::string val = std::string(1,str[1]) + std::string(1,str[2]);
        if(val == "AX") {
          regRet = reg::R0;
          sizeRet = size::WORD;
        } else if(val == "CX") {
          regRet = reg::R1;
          sizeRet = size::WORD;
        } else if(val == "DX") {
          regRet = reg::R2;
          sizeRet = size::WORD;
        } else if(val == "BX") {
          regRet = reg::R3;
          sizeRet = size::WORD;
        } else if(val == "SP") {
          regRet = reg::R4;
          sizeRet = size::WORD;
        } else if(val == "BP") {
          regRet = reg::R5;
          sizeRet = size::WORD;
        } else if(val == "SI") {
          regRet = reg::R6;
          sizeRet = size::WORD;
        } else if(val == "DI") {
          regRet = reg::R7;
          sizeRet = size::WORD;
        } else if(val == "AL") {
          regRet = reg::R0;
          sizeRet = size::BYTE;
        } else if(val == "CL") {
          regRet = reg::R1;
          sizeRet = size::BYTE;
        } else if(val == "DL") {
          regRet = reg::R2;
          sizeRet = size::BYTE;
        } else if(val == "BL") {
          regRet = reg::R3;
          sizeRet = size::BYTE;
        } else if(val == "AH") {
          regRet = reg::R4;
          sizeRet = size::BYTE;
        } else if(val == "CH") {
          regRet = reg::R5;
          sizeRet = size::BYTE;
        } else if(val == "DH") {
          regRet = reg::R6;
          sizeRet = size::BYTE;
        } else if(val == "BH") {
          regRet = reg::R7;
          sizeRet = size::BYTE;
        }
      } else {
        uniform = true;
        std::string val = std::string(1,str[1]) + std::string(1,str[2]) + std::string(1,str[3]);
        if(val == "SPL") {
          regRet = reg::R4;
          sizeRet = size::BYTE;
        } else if(val == "BPL") {
          regRet = reg::R5;
          sizeRet = size::BYTE;
        } else if(val == "SIL") {
          regRet = reg::R6;
          sizeRet = size::BYTE;
        } else if(val == "DIL") {
          regRet = reg::R7;
          sizeRet = size::BYTE;
        }
      }
    }
  }
}





size char_to_size(char quant, bool fpu, bool fpuInt) {
  if(quant == 'b') {
    if(fpu) return size::INVALID;
    return size::BYTE;
  } else if(quant == 'w') {
    if(fpu && !fpuInt) return size::INVALID;
    return size::WORD;
  } else if(quant == 's') {
    if(fpu && !fpuInt) return size::DOUBLE_WORD;
    return size::WORD;
  } else if(quant == 'd') {
    return size::DOUBLE_WORD;
  } else if(quant == 'l') {
    if(fpu && !fpuInt) return size::QUAD_WORD;
    return size::DOUBLE_WORD;
  } else if(quant == 'q') {
    return size::QUAD_WORD;
  } else if(quant == 't') {
    if(fpu && fpuInt) return size::INVALID;
    return size::TEN_BYTE;
  } else {
    return size::NO_SUCH_SIZE;
  }
}
