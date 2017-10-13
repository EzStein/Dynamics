/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "compiler/asm/assembler.h"
#include "compiler/lex/token.h"
#include "compiler/lex/lexer.h"
#include <algorithm>
#include <cctype>
#include <string>

std::vector<unsigned char> assembler::assemble(std::string str) {
  map<string, token> lexDef;
  lexDef[string("\\s*\\(")] = token::LEFT_PAREN;
  lexDef[string("\\s*\\)")] = token::RIGHT_PAREN;
  lexDef[string("\\s*\\n")] = token::NEW_LINE;
  lexDef[string("\\s*$")] = token::DOLLAR_SIGN;
  lexDef[string("\\s*0x[ABCDEFabcdef\\d][ABCDEFabcdef\\d]*")] = token::HEX_INT;
  lexDef[string("\\s*(-\\d|\\d)\\d*")] = token::DEC_INT;
  lexDef[string("\\s*%[\\a\\A\\d][\\a\\A\\d]*")] = token::REG;
  lexDef[string("\\s*[\\a\\A\\d][\\a\\A\\d]*")] = token::INSTRUCTION;
  
  std::stringstream stream(str);
  lexer lex = lexer(&stream, lexDef, false);
  std::string lexeme;
  token tok;
  tok = lex.next_token(lexeme);
  /*Remove the whitespace from the string*/
  lexeme.erase(std::remove_if(lexeme.begin(), lexeme.end(), std::isspace), lexeme.end());
  
  if(tok != token::INSTRUCTION)
    throw "Unexpected token: " + token_to_string(tok) + ", " + lexeme + " Expected Instruction";
  /*Make upper case*/
  std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::toupper);
  
}

void assembler::append(instruction inst, size operandSize, size addressSize,
  addressing_mode addrMode, reg rmReg, long rmDisp,
  reg rReg, bool isImm, long imm, bool rmIsDest, bool uniformReg,
  std::vector<unsigned char>& buf) {

  /*We assume we are in 64 bit long mode. The default operand size is 32 bits while the default address size is
   64 bits. We first construct the REX prefix. We need it under three circumstances.
   If we are using a 64 bit operand size if
   rmReg or rReg is R8 - R15, or if we are using SPL BPL DIL SIL uniform byte reg's*/

  unsigned char rexPrefix = 0x40;
  if(operandSize == size::QUAD_WORD) {
    rexPrefix |= 0x08;
  }
  if(rReg >= 8) {
    rexPrefix |= 0x04;
  }
  if(rmReg >= 8) {
    rexPrefix |= 0x01;
  }
  if(operandSize == size::QUAD_WORD || rReg >= 8 || rmReg >= 8
    || (operandSize == size::BYTE && uniformReg && (rReg >= 4 && rReg <= 7 || rmReg >= 4 && rmReg <= 7 ))) {
    buf.push_back(rexPrefix);
  }

  /*If the operand size is 16 bits, we use an override prefix*/
  if(operandSize == size::WORD) {
    buf.push_back(0x66);
  }
  /*If the operand size is 8 bits, that info is encoded into the instruction opcode*/
  /*If the addressSize is 32 bits we use an override prefix*/
  if(addressSize == size::DOUBLE_WORD) {
    buf.push_back(0x67);
  }

  /*We now construct the primary opcode and opcode extension. The
   opcode extension is inserted into the modrmbyte if the insertOpExt bool is set to true.
   We skip the modrmByte calculation if the modrmBytePreCalculated bool is set to true*/
  unsigned char opcode;
  unsigned char opExt;
  bool insertOpExt = false;
  bool modrmBytePreCalculated = false;


  if(inst > instruction::FPU_STD_ARITH_INSTRUCTION_START && inst < instruction::FPU_STD_ARITH_INSTRUCTION_END) {
     /*We will first implement FPU standard arithmetic instructions.
   * The format for the opcode is 0xD8 (bit 3 is always 1, bit 0 is always 0).
   * Bit 2 - Determines the memory operand size. For integer instructions
   * 0 indicates 32 bit, and 1 indicates 16 bit. For non integer instructions
   * 0 indicates 32 bit and 1 indicates 64 bit.
   * Bit 1 - Indicates an integer operation if 1.
   * The reg field indicates an opcode extension. So we set insertOpExt to true
   *
   * For all FPU arithmetic instructions
   that accept two operands (fadd fmul fdiv fdivr fsub fsubr fcom fcomp),
   * The reg field is always an opcode extension. When using integer operations that
   * do not access memory (mod = 11) then the instruction is special whose purpose
   * depends on bit 2. If bit 2 is 0 then we have an FCMOV operation.
   * If bit 2 is 1 then we have a pop after the operation. For non integer
   * operations that do not use memory (mod = 11), bit 2 determines the direction
   * of the operation. If bit 2 is 0 we have st0 as destination. Otherwise we have
   * op1 as destination.
   *
   * If a FPU instruction is passed to this function,
   * the operand size may be 16 bit (valid only for integers), 32 bit or 64 bit (valid only for non integers).
    There is only
   * one operand. Always! rReg is always ignored. That operand is specified using the rmReg,
   * addressing mode, and rmDisp. If the addressing mode is not REG,
   * then then rmIsDest is ignored. If it is reg, then it determines whether the
   * destination is st0 or some other register. isImm and imm are always ignored.
   */
    insertOpExt = true;
    opcode = 0xD8;
    if(is_fpu_integer_instruction(inst) || is_fpu_pop_instruction(inst)) {
      opcode |= 0x02;
    }
    if(is_fpu_integer_instruction(inst) && addrMode != addressing_mode::MEM) {
      throw "FPU integer instructions must access memory";
    }
    if(addrMode == addressing_mode::MEM) {
      switch(operandSize) {
        case size::WORD:
          if(!is_fpu_integer_instruction(inst)) throw "FPU word size only supported for integer instructions";
          opcode |= 0x04;
          break;
        case size::DOUBLE_WORD:
          opcode |= 0x00;
          break;
        case size::QUAD_WORD:
          if(is_fpu_integer_instruction(inst)) throw "FPU QUAD_WORD size only supported for floating vals!";
          opcode |=0x04;
        default: throw "Byte size not supported in FPU instructions!";
      }
    } else {
      /*In this case, bit 2 determines the destination register and the operand size is ignored.*/
      if(rmIsDest) {
        opcode |= 0x04;
      } else {
        opcode |= 0x00;
      }
    }
    /*We now set the low 3 bits of opExt*/
    switch(inst) {
      case instruction::FADD:
      case instruction::FADDP:
      case instruction::FIADD:
        opExt = 0x00; break;
      case instruction::FMUL:
      case instruction::FMULP:
      case instruction::FIMUL:
        opExt = 0x01; break;
      case instruction::FSUB:
      case instruction::FSUBP:
      case instruction::FISUB:
        opExt = 0x04; break;
      case instruction::FSUBR:
      case instruction::FSUBRP:
      case instruction::FISUBR:
        opExt = 0x05; break;
      case instruction::FDIV:
      case instruction::FDIVP:
      case instruction::FIDIV:
        opExt = 0x06; break;
      case instruction::FDIVR:
      case instruction::FDIVRP:
      case instruction::FIDIVR:
        opExt = 0x07; break;
      default: throw "THIS SHOULD NOT OCCUR";
    }
    buf.push_back(opcode);
  } else if(inst > instruction::FPU_NO_OPERAND_ARITH_INSTRUCTION_START &&
    inst < instruction::FPU_NO_OPERAND_ARITH_INSTRUCTION_END) {
    /* These instructions are easy. Since they have no operand, the modrmbyte
     * is used to specify the exact instruction. When one of
     * these instructions is encountered, all other parameters passed to this
     * function are ignored. Additionally, we set a flag indicating that
     * the modrmbyte has already been calculated and should not be recalculated
     * later on. We do not need to set the insertOpExt bool because that step will be skipped.
     */
    
    modrmBytePreCalculated = true;
    unsigned char modrmByte = 0x00;
    switch(inst) {

      case instruction::FNOP:
        opcode = 0xD9;
        modrmByte = 0b11010000;
        break;
      case instruction::FCHS:
        opcode = 0xD9;
        modrmByte = 0xE0;
        break;
      case instruction::FABS:
        opcode = 0xD9;
        modrmByte = 0xE1;
        break;
      case instruction::FTST:
        opcode = 0xD9;
        modrmByte = 0xE4;
        break;
      case instruction::FXAM:
        opcode = 0xD9;
        modrmByte = 0xE5;
        break;
      case instruction::FLD1:
        opcode = 0xD9;
        modrmByte = 0xE8;
        break;
      case instruction::FLDL2T:
        opcode = 0xD9;
        modrmByte = 0xE9;
        break;
      case instruction::FLDL2E:
        opcode = 0xD9;
        modrmByte = 0xEA;
        break;
      case instruction::FLDPI:
        opcode = 0xD9;
        modrmByte = 0xEB;
        break;

      case instruction::FLDLG2:
        opcode = 0xD9;
        modrmByte = 0xEC;
        break;
      case instruction::FLDLN2:
        opcode = 0xD9;
        modrmByte = 0xED;
        break;
      case instruction::FLDZ:
        opcode = 0xD9;
        modrmByte = 0xEE;
        break;
      case instruction::F2XM1:
        opcode = 0xD9;
        modrmByte = 0xF0;
        break;
      case instruction::FY2LX:
        opcode = 0xD9;
        modrmByte = 0xF1;
        break;
      case instruction::FPTAN:
        opcode = 0xD9;
        modrmByte = 0xF2;
        break;
      case instruction::FPATAN:
        opcode = 0xD9;
        modrmByte = 0xF3;
        break;
      case instruction::FXTRACT:
        opcode = 0xD9;
        modrmByte = 0xF4;
        break;
      case instruction::FPREM1:
        opcode = 0xD9;
        modrmByte = 0xF5;
        break;

      case instruction::FDECSTP:
        opcode = 0xD9;
        modrmByte = 0xF6;
        break;
      case instruction::FINCSTP:
        opcode = 0xD9;
        modrmByte = 0xF7;
        break;
      case instruction::FPREM:
        opcode = 0xD9;
        modrmByte = 0xF8;
        break;
      case instruction::FY2LXP1:
        opcode = 0xD9;
        modrmByte = 0xF9;
        break;
      case instruction::FSQRT:
        opcode = 0xD9;
        modrmByte = 0xFA;
        break;
      case instruction::FSINCOS:
        opcode = 0xD9;
        modrmByte = 0xFB;
        break;
      case instruction::FRNDINT:
        opcode = 0xD9;
        modrmByte = 0xFC;
        break;

      case instruction::FSCALE:
        opcode = 0xD9;
        modrmByte = 0xFD;
        break;
      case instruction::FSIN:
        opcode = 0xD9;
        modrmByte = 0xFE;
        break;
      case instruction::FCOS:
        opcode = 0xD9;
        modrmByte = 0xFF;
        break;
      case instruction::FUCOMPP:
        opcode = 0xDA;
        modrmByte = 0xE9;
        break;
      case instruction::FNCLEX:
        opcode = 0xDB;
        modrmByte = 0xE2;
        break;
      case instruction::FNINIT:
        opcode = 0xDB;
        modrmByte = 0xE3;
        break;
      case instruction::FNSTSW:
        opcode = 0xDF;
        modrmByte = 0xE0;
        break;
      default: throw "THIS SHOULD NOT OCCUR";
    }
    buf.push_back(opcode);
    buf.push_back(modrmByte);
  } else if(inst > instruction::FPU_MEM_INSTRUCTION_START && inst < instruction::FPU_MEM_INSTRUCTION_END) {
    /*For store instructions, no pop indicates opExt is 010 and pop indicates opExt is 011
     * or 111 depending on the operand size.
     * For load instructions, opExt 000 indicates normal load while opExt 101 indicates
     * is used depending on the operand size.
     * If it is an integer operation, bit 1 is set to 1, otherwise it is set to 0.
     * Bit 2 indicates the operand size. For all instructions 0 means that it is 32 bits.
     * For integer instructions, 1 means 16 bits and for non integer instructions 1 means 64 bits.
     * Bit 0 is always set to 1.
     * Integer instructions require the addressing mode to be mem while. For non integer
     * stores the addressing mode may be REG ONLY when the opcode is 0xDD.
     * opExt 111 is used to indicate a store pop instruction
     * whose width is 64 bits if bit 2 is 1 for integer operations and 80 bits if bit 2 is 0.
     * 
     */
    insertOpExt = true;
    opcode = 0xD9;
    /*First we set the bit 1 if it is an integer operation*/
    if(is_fpu_integer_instruction(inst)) {
      opcode |= 0x02;
    }
    
    /*Now we set the opExt*/
    if(is_fpu_load_instruction(inst)) {
      opExt = 0x00;
    } else if(is_fpu_pop_instruction(inst)) {
      opExt = 0x03;
    } else {
      opExt = 0x02;
    }
    
    
    /*Now we calculate bit 2 and possibly override opExt*/
    if(operandSize == size::DOUBLE_WORD) {
      /*32 bits are always set to 0*/
      opcode |= 0x00;
    } else if(operandSize == size::BYTE) {
      throw "FPU instructions cannot use a BYTE operand size";
    } else if(operandSize == size::WORD) {
      /*If this is an integer instruction we set 1, otherwise we throw an exception*/
      if(!is_fpu_integer_instruction(inst)) {
        throw "FPU non-integer instructions cannot use a WORD operand size";
      }
      opcode |= 0x04;
    } else if(operandSize == size::QUAD_WORD) {
      /*We always set bit 1. If it is an integer instruction, we override opExt to be 111*/
      opcode |=0x04;
      if(is_fpu_integer_instruction(inst)) {
        if(is_fpu_load_instruction(inst))
          opExt = 0x05;
        else
          opExt = 0x07;
      }
    } else if(operandSize == size::TEN_BYTE) {
      if(is_fpu_integer_instruction(inst)) {
        throw "FPU integer instructions cannot use TEN_BYTE operand size";
      }
      /*We DO NOT set bit 2 to 1, but we do override the opEXT*/
      opcode |= 0x00;
      if(is_fpu_load_instruction(inst))
        opExt = 0x05;
      else
        opExt = 0x07;
    }
    buf.push_back(opcode);
  } else if(inst > instruction::STD_ARITH_INSTRUCTION_START && inst < instruction::STD_ARITH_INTSRUCTION_END) {
    /*For these instructions, bit 8 determines whether or not
     * there is an immediate. If there is, it is set and rReg is ignored. The reg field
     * of the modrmbyte is encodes the type of instruction.
     * Bit 0 indicates
      The opExt is also set here. The following switch case
      should only assign zero to bits 0, 1, and 7 as these will be determined later*/
     switch(inst) {
       case instruction::ADD: 
         opcode = 0x00;
         opExt = 0x00;
         break;
       case instruction::OR: 
         opcode = 0x08;
         opExt = 0x01;
         break;
       case instruction::ADC: 
         opcode = 0x10;
         opExt = 0x02;
         break;
       case instruction::SBB:
         opcode = 0x18;
         opExt = 0x03;
         break;
       case instruction::AND:
         opcode = 0x20;
         opExt = 0x04;
         break;
       case instruction::SUB:
         opcode = 0x28;
         opExt = 0x05;
         break;
       case instruction::XOR:
         opcode = 0x30;
         opExt = 0x06;
         break;
       case instruction::CMP:
         opcode = 0x38;
         opExt = 0x07;
         break;
       default: break;
     };
     /*If imm is set, we set the immediate*/
    if(isImm) {
      insertOpExt = true;
      /*The base opcode is 0x80 for immediate values*/
      opcode = 0x80;

       /*IMM. If the operand size is 8 bits we set the sign extend bit to 0 and we require that
      the immediate fit into 8 bits. If the operand size is not 8 bits we do the following:
      if the immediate can be fit into 8 bits, we do it and then set the sign extend bit.
      Otherwise we set the sign extend bit to 0 and require that the immediate fit
      the operand size. The number of bytes used is the operand size OR 4 bytes if the operand
      size is quad word*/
       if(operandSize == size::BYTE) {
         opcode |= 0x00;
       } else {
         if(imm >= -128 && imm <= 127) {
           opcode |= 0x02;
         } else {
           opcode |= 0x00;
         }
       }
     } else {
        /*If there is no immediate then bit 1 determines the direction*/
        if(rmIsDest) {
          opcode |= 0x00;
        } else {
          opcode |= 0x02;
        }
     }

     /*Now we set the operand width bit. If the operand width is 8 bits, we set it to 0
      otherwise we set it to 1*/
     if(operandSize == size::BYTE) {
       opcode |= 0x00;
     } else {
       opcode |= 0x01;
     }
     buf.push_back(opcode);
  }

  /*We now construct the mod/rm byte. Note that addressing the EBP/RBP requires a mod of 01 or 10 that is a
   * potentially zero displacement. Addressing the ESP/RSP requires a SIB byte  */
  unsigned char modrmByte = 0x00;

  /*If addrMode is REG then things are simple. The first two bits are 11.
   The next 3 specify rReg, and the next 3 specify rmReg*/
  if(addrMode == addressing_mode::REG) {
    modrmByte |= 0xC0;
  } else if(addrMode == addressing_mode::MEM) {
    /*If rmReg is bp or r13 we do something special, otherwise the
     first two bits are either 00, 01, 10 depending on the displacement size*/
    if(rmReg == reg::R5 || rmReg == reg::R13) {
      /*In this case, 00 bits indicate RIP relative addressing, so we always use
       a possibly zero displacement byte.*/
      if(rmDisp >= -128 && rmDisp <= 127) {
        modrmByte |= 0x40;
      } else {
        modrmByte |= 0x80;
      }
    } else {
      /*If the displacement is zero, the first two bits are 00, so we do nothing*/
      /*If the displacement fits in 8 bits we use 01, otherwise, 10*/
      if(rmDisp == 0) {

      } else if(rmDisp >= -128 && rmDisp <= 127) {
        modrmByte |= 0x40;
      } else {
        modrmByte |= 0x80;
      }
    }
  }

  /*The reg field (middle 3 bits) are the low 3 bits of rReg unless an imm is used
   in which case it is an opcode extension. the last 3 bits are the rmReg field*/
  if(insertOpExt) {
    modrmByte |= opExt << 3;
  } else {
    modrmByte |= ((rReg & 0x07) << 3);
  }
  modrmByte |= rmReg & 0x07;
  /*We add the modrmByte if it was not already added*/
  if(!modrmBytePreCalculated)
    buf.push_back(modrmByte);

  /*Now if rmReg is sp or r12, we need to specify a SIB byte that
   evaluates as sp or r12*/
  if(rmReg == reg::R4 || rmReg == reg::R12) {
    unsigned char sib = 0b00100000;
    /*The first two bits (00) indicate a scale of 1.
     The next 3 (100) indicate that there is no index
     and the next 3 indicate the base register*/
    sib |= rmReg & 0x07;
    buf.push_back(sib);
  }

  /*Now we encode the displacement bytes. If rmReg is r5 or r13 then we always
   encode a byte even when disp is zero*/
  if(rmDisp != 0 || rmReg == reg::R5 || rmReg == reg::R13) {
    if(rmDisp >= -128 && rmDisp <= 127) {
      buf.push_back(rmDisp);
    } else {
      buf.push_back(rmDisp);
      buf.push_back(rmDisp >> 8);
      buf.push_back(rmDisp >> 16);
      buf.push_back(rmDisp >> 24);
    }
  }

  /*IMM. If the operand size is 8 bits we set the sign extend bit to 0 and we require that
   the immediate fit into 8 bits. If the operand size is not 8 bits we do the following:
   if the immediate can be fit into 8 bits, we do it and then set the sign extend bit.
   Otherwise we set the sign extend bit to 0 and require that the immediate fit
   the operand size. The number of bytes used is the operand size OR 4 bytes if the operand
   size is quad word*/
  if(isImm) {
    if(operandSize == size::BYTE) {
      if(imm >= -128 && imm <= 127) {
        buf.push_back(imm);
      } else {
        throw "IMM of byte operand does not fit in one byte!";
      }
    } else {
      if(imm >= -128 && imm <= 127) {
        buf.push_back(imm);
      } else {
        /*We fit it into the operand size*/
        if(operandSize == size::WORD) {
          if(imm >= -32768 && imm <= 32767) {
            buf.push_back(imm);
            buf.push_back(imm >> 8);
          } else {
            throw "IMM of word operand does not fit in one word!";
          }
        } else if(operandSize == size::DOUBLE_WORD || operandSize == size::QUAD_WORD) {
          if(imm >= -2,147,483,648 && imm <= 2,147,483,647) {
            buf.push_back(imm);
            buf.push_back(imm >> 8);
            buf.push_back(imm >> 16);
            buf.push_back(imm >> 24);
          } else {
            throw "IMM of DOUBLE WORD operand does not fit in DOUBLE WORD!";
          }
        }
      }
    }
  }
}
