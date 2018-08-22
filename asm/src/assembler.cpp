/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "asm/assembler.h"

#include "regex/lexer_builder.h"
#include "regex/lexer.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
namespace dynsolver {

std::vector<unsigned char> assembler::assemble(std::string str) {
  regex::lexer lex = regex::lexer_builder()
      .register_rule(" *,",token::COMMA)
      .register_rule(" *\\(",token::LEFT_PAREN)
      .register_rule(" *\\)",token::RIGHT_PAREN)
      .register_rule(" *\n",token::NEW_LINE)
      .register_rule(" *$",token::DOLLAR_SIGN)
      .register_rule(" *(0x|-0x)[ABCDEFabcdef\\d][ABCDEFabcdef\\d]*",token::HEX_INT)
      .register_rule(" *(-\\d|\\d)\\d*",token::DEC_INT)
      .register_rule(" *%[\\w][\\w]*",token::REG)
      .register_rule(" *%st\\(\\d\\)",token::REG)
      .register_rule(" *[\\w][\\w]*",token::INSTRUCTION).build();

  lex.set_input(str);

  std::vector<unsigned char> vec;
  int lineNo = 0;
  while(1) {

    ++lineNo;
    std::string lexeme;
    int tok;
    tok = lex.consume_token(lexeme);


    if(tok != token::INSTRUCTION)
      throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token: "
        + token_to_string(tok) + ", " + lexeme + " Expected Instruction";
    /*Remove the whitespace from the string*/
    lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    /*Make upper case*/
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
    instruction inst = str_to_instruction(lexeme);
    size operandSize = size::NONE;
    /*If the instruction does not exist, then we assume
     that there is a quantifier specifying the operand size after it*/
    if(inst == instruction::NO_SUCH_INSTRUCTION) {

      /*Note that the string is nonempty, so we can do:*/
      char quantifier = *(lexeme.end() - 1);
      /*We remove the last letter of the instruction*/
      lexeme.pop_back();
      inst = str_to_instruction(lexeme);
      if(inst == instruction::NO_SUCH_INSTRUCTION)
        throw "On line, "+ std::to_string(lineNo) + ": " + "No such instruction: " + lexeme + quantifier;
      operandSize = char_to_size(quantifier, is_fpu_instruction(inst), is_fpu_integer_instruction(inst));


      if(operandSize == size::NO_SUCH_SIZE)
        throw "On line, "+ std::to_string(lineNo) + ": " + "No such size specifier: " + quantifier;
      if(operandSize == size::INVALID)
        throw "On line, "+ std::to_string(lineNo) + ": " + "Quantifier '" + std::string(1, quantifier) + "' is not valid in this context.";
    }
    /*We now have an instruction and operand size which may be NONE.
     If it is NONE, we will construct it from the operands*/
    tok = lex.consume_token(lexeme);

    /*Remove the whitespace from the string*/
    lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    /*Make upper case*/
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
    reg rReg = reg::NONE, rmReg = reg::NONE;
    addressing_mode addrMode = addressing_mode::NONE;
    size addressSize = size::QUAD_WORD;
    reg_type regType = reg_type::GPR;
    long rmDisp;
    bool isImm = false;
    long imm;
    bool rmIsDest;
    bool uniformReg;
    if(tok == token::NEW_LINE || tok == token::ENDPOINT) {
      append(inst, operandSize, addressSize, regType, addrMode, rmReg, rmDisp, rReg, isImm, imm, rmIsDest, uniformReg, vec);

      if(tok == token::ENDPOINT) break;
      if(tok == token::NEW_LINE) continue;
    }
    /*If there are two operands, the second one (destination) will be
     assigned rmReg field if possible, and rmIsDest will be true.
     * The only instance when this does not
     occur is when source is a memory operand or when both
     * are FPU registers. In that case, the one which is NOT
     * the zero register will be put in the rmReg field.*/
    /*If there is only one operand that operand is kept in the rmReg field*/

    if(tok == token::REG) {
      /*We have encountered a REG, We convert it to a register
       and verify that it's size is appropriate. If no quantifier was given,
       we set the quantifier.*/

      /*
       * This reg will always be assigned to rmReg.
       * If it turns out that the next operand is a memory operand,
       * we will move this reg to rReg and use the rmReg appropriately.
       *
       * If the next operand is just a reg, it is put into rReg,
       * unless it is an FPU st register that is not ST0, or we are using an imm.
       */
      size size = size::NONE;
      str_to_reg(lexeme, rmReg, regType, size, uniformReg);
      if(rmReg == reg::NONE) throw "No such register " + lexeme;
      if(operandSize == size::NONE) operandSize = size;
      else if(regType == reg_type::GPR && operandSize != size) {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Operand size mismatch!";
      }
      rmIsDest = false;
      addrMode = addressing_mode::REG;
    } else if(tok == token::DOLLAR_SIGN) {
      tok = lex.consume_token(lexeme);
      /*Remove the whitespace from the string*/
      lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
      /*Make upper case*/
      std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
      isImm = true;

      if(tok == token::HEX_INT) {
        imm = std::stoul(lexeme, nullptr, 16);
      } else if(tok == token::DEC_INT) {
        imm = std::stol(lexeme);
      } else {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + lexeme + "' Expected integer literal.";
      }
    } else {
      /*We have encountered a memory operand*/
      if(tok == token::HEX_INT) {
        rmDisp = std::stoul(lexeme, nullptr, 16);
        /*Consume the left Paren*/
        if(lex.consume_token(lexeme) != token::LEFT_PAREN) throw "On line, "+ std::to_string(lineNo) + ": " +  "Unexpected token '" + lexeme + "' Expected '('";
      } else if(tok == token::DEC_INT) {
        rmDisp = std::stol(lexeme);
        if(lex.consume_token(lexeme) != token::LEFT_PAREN) throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + lexeme + "' Expected '('";
      } else if(tok == token::LEFT_PAREN) {
        rmDisp = 0;
      } else {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + std::to_string(tok) + "' Expected register or memory operand";
      }
      tok = lex.consume_token(lexeme);
      /*Remove the whitespace from the string*/
      lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
      /*Make upper case*/
      std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
      size size = size::NONE;
      bool dummy;
      str_to_reg(lexeme, rmReg, regType, size, dummy);
      if(regType != reg_type::GPR) {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Only general purpose registers may be used for addressing";
      }
      if(size == size::BYTE || size == size::WORD) {
        throw "On line, "+ std::to_string(lineNo) + ": " + "8 bit and 16 bit addressing are not supported in long mode!";
      }
      addressSize = size;
      addrMode = addressing_mode::MEM;
      rmIsDest = false;
      lex.consume_token(lexeme); //Consume the right Paren

    }

    tok = lex.consume_token(lexeme);
    /*Remove the whitespace from the string*/
    lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    /*Make upper case*/
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
    if(tok == token::NEW_LINE || tok == token::ENDPOINT) {
      append(inst, operandSize, addressSize, regType, addrMode, rmReg, rmDisp, rReg, isImm, imm, rmIsDest, uniformReg, vec);

      if(tok == token::ENDPOINT) break;
      if(tok == token::NEW_LINE) continue;
    }

    if(tok != token::COMMA) throw std::string("On line, "+ std::to_string(lineNo)
                                + ": " + "Unexpected token '" + lexeme
                                + "' (" + token_to_string(tok) + ") Expected ','");

    tok = lex.consume_token(lexeme);
    /*Remove the whitespace from the string*/
    lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    /*Make upper case*/
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);

    reg_type tmpRegType;
    if(tok == token::REG) {
      /*We have encountered a REG, We convert it to a register
       and verify that it's size is appropriate. If no quantifier was given,
       we set the quantifier.*/

      size size = size::NONE;

      str_to_reg(lexeme, rReg, tmpRegType, size, uniformReg);
      //if(uniformReg != tmpUniformReg) throw "Uniform byte registers must be used with other uniform registers";
      if(rReg == reg::NONE) throw "On line, "+ std::to_string(lineNo) + ": " + "No such register " + lexeme;
      if(operandSize == size::NONE) operandSize = size;
      else if(tmpRegType == reg_type::GPR && operandSize != size) {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Operand size mismatch!";
      }
      /*If we are using an immediate, we must use the modrm byte*/
      if(isImm || (tmpRegType == reg_type::FPU && rReg != reg::R0)) {
        rmReg = rReg;
        addrMode = addressing_mode::REG;
        rmIsDest = true;
      }
    } else if(tok == token::DOLLAR_SIGN) {
      throw "On line, "+ std::to_string(lineNo) + ": " + "An immediate operand can not be a destination";
    } else {
      /*We have encountered a memory operand*/
      if(tok == token::HEX_INT) {
        rmDisp = std::stoul(lexeme, nullptr, 16);
        /*Consume the left Paren*/
        if(lex.consume_token(lexeme) != token::LEFT_PAREN) throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + lexeme + "' Expected '('";
      } else if(tok == token::DEC_INT) {
        rmDisp = std::stol(lexeme);
        if(lex.consume_token(lexeme) != token::LEFT_PAREN) throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + lexeme + "' Expected '('";
      } else if(tok == token::LEFT_PAREN) {
        rmDisp = 0;
      } else {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + lexeme + "' Expected register or memory operand";
      }
      tok = lex.consume_token(lexeme);
      /*Remove the whitespace from the string*/
      lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
      /*Make upper case*/
      std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);

      size size = size::NONE;
      bool dummy;
      /*We move whatever was in rmReg to rReg. There cannot be two memory operands, so this is ok*/
      rReg = rmReg;
      str_to_reg(lexeme, rmReg, tmpRegType, size, dummy);
      if(tmpRegType != reg_type::GPR) {
        throw "On line, "+ std::to_string(lineNo) + ": " + "Only general purpose registers may be used for addressing";
      }
      if(size == size::BYTE || size == size::WORD) {
        throw "On line, "+ std::to_string(lineNo) + ": " + "8 bit and 16 bit addressing are not supported in long mode!";
      }
      addressSize = size;
      addrMode = addressing_mode::MEM;
      rmIsDest = true;
      lex.consume_token(lexeme); //Consume the right Paren
    }

    tok = lex.consume_token(lexeme);
    /*Remove the whitespace from the string*/
    lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), ' '), lexeme.end());
    /*Make upper case*/
    std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);

    if(tmpRegType == reg_type::MMX)
      regType = reg_type::MMX;
    else if(tmpRegType == reg_type::XMM)
      regType = reg_type::XMM;
    else if(tmpRegType == reg_type::FPU)
      regType = reg_type::FPU;

    append(inst, operandSize, addressSize, regType, addrMode, rmReg, rmDisp, rReg, isImm, imm, rmIsDest, uniformReg, vec);

    if(tok == token::ENDPOINT) break;
    if(tok != token::NEW_LINE) throw "On line, "+ std::to_string(lineNo) + ": " + "Unexpected token '" + lexeme + "' Expected '\\n'";
  }
  return vec;
}

void assembler::append(instruction inst, size operandSize, size addressSize, reg_type regType,
  addressing_mode addrMode, reg rmReg, long rmDisp,
  reg rReg, bool isImm, long imm, bool rmIsDest, bool uniformReg,
  std::vector<unsigned char>& buf) {

   /*If the operand size is 16 bits, we use an override prefix*/
  if(operandSize == size::WORD) {
    buf.push_back(0x66);
  }
  /*If the operand size is 8 bits, that info is encoded into the instruction opcode*/
  /*If the addressSize is 32 bits we use an override prefix*/
  if(addressSize == size::DOUBLE_WORD) {
    buf.push_back(0x67);
  }

  /*We assume we are in 64 bit long mode. The default operand size is 32 bits while the default address size is
   64 bits. We first construct the REX prefix. We need it under three circumstances.
   If we are using a 64 bit operand size (not a FPU operation), if
   rmReg or rReg is R8 - R15, or if we are using SPL BPL DIL SIL uniform byte reg's*/

  int rRegIntVal = static_cast<int>(rReg);
  int rmRegIntVal = static_cast<int>(rmReg);
  unsigned char rexPrefix = 0x40;
  if(operandSize == size::QUAD_WORD && inst != instruction::RETN && !is_fpu_instruction(inst) && inst != instruction::POP && inst != instruction::PUSH) {
    rexPrefix |= 0x08;
  }
  if(rRegIntVal >= 8 && rReg != reg::NONE) {
    rexPrefix |= 0x04;
  }
  if(rmRegIntVal >= 8 && rmReg != reg::NONE) {
    rexPrefix |= 0x01;
  }
  if((operandSize == size::QUAD_WORD && inst != instruction::RETN && inst != instruction::POP && inst != instruction::PUSH && !is_fpu_instruction(inst) && regType == reg_type::GPR) ||
    (rRegIntVal >= 8 && rReg != reg::NONE) || (rmRegIntVal >= 8 && rmReg != reg::NONE)
    || (operandSize == size::BYTE && uniformReg && (rRegIntVal >= 4 && rRegIntVal <= 7 || rmRegIntVal >= 4 && rmRegIntVal <= 7 ))) {

      buf.push_back(rexPrefix);
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
          break;
        default: throw "Operand size must be specified in FPU memory instructions!";
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
        if(addrMode == addressing_mode::REG)
          opExt = 0x05;
        else
          opExt = 0x04;
        break;
      case instruction::FSUBR:
      case instruction::FSUBRP:
      case instruction::FISUBR:
       if(addrMode == addressing_mode::REG)
          opExt = 0x04;
        else
          opExt = 0x05;
        break;
      case instruction::FDIV:
      case instruction::FDIVP:
      case instruction::FIDIV:
        if(addrMode == addressing_mode::REG)
          opExt = 0x07;
        else
          opExt = 0x06;
        break;
      case instruction::FDIVR:
      case instruction::FDIVRP:
      case instruction::FIDIVR:
        if(addrMode == addressing_mode::REG)
          opExt = 0x06;
        else
          opExt = 0x07;
        break;
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
        modrmByte = 0xD0;
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
      case instruction::FYL2X:
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
      case instruction::FPREM:
        opcode = 0xD9;
        modrmByte = 0xF8;
        break;
      case instruction::FYL2XP1:
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
      case instruction::FINCSTP:
        opcode =0xD9;
        modrmByte = 0xF7;
        break;
      case instruction::FDECSTP:
        opcode = 0xD9;
        modrmByte = 0xF6;
        break;
      case instruction::FINIT:
        buf.push_back(0x9B);
        opcode = 0xDB;
        modrmByte = 0xE3;
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
      if(is_fpu_load_instruction(inst)) {
        opcode = 0xDB;
        opExt = 0x05;
      } else {
        opcode = 0xDB;
        opExt = 0x07;
      }
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
  } else if(inst == instruction::MOV) {
    if(regType != reg_type::GPR) {
      if(operandSize == size::QUAD_WORD) {
        if(regType == reg_type::MMX && rmIsDest) {
          buf.push_back(0x0F);
          buf.push_back(0x7F);
        } else if(regType == reg_type::MMX && !rmIsDest) {
          buf.push_back(0x0F);
          buf.push_back(0x6F);
        } else if(regType == reg_type::XMM && rmIsDest) {
          buf.push_back(0x66);
          buf.push_back(0x0F);
          buf.push_back(0xD6);
        } else if(regType == reg_type::XMM && !rmIsDest) {
          buf.push_back(0xF3);
          buf.push_back(0x0F);
          buf.push_back(0x7E);
        }
      } else if(operandSize == size::DOUBLE_WORD) {
        if(regType == reg_type::XMM) {
          buf.push_back(0x66);
        }
        buf.push_back(0x0F);
        if(rmIsDest) {
          buf.push_back(0x7E);
        } else {
          buf.push_back(0x6E);
        }
      }


    } else {

      /*We have a GPR*/
      unsigned char opcode;
      if(isImm) {
        opcode = 0xC6;
        opExt = 0x00;
        insertOpExt = true;
      } else {
        opcode = 0x88;
        if(rmIsDest) {
          opcode |= 0x00;
        } else {
          opcode |= 0x02;
        }
      }
      if(operandSize == size::BYTE) {
        opcode |= 0x00;
      } else {
        opcode |= 0x01;
      }

      buf.push_back(opcode);
    }

  } else if(inst == instruction::POP) {

    buf.push_back(0x8F);
    opExt = 0x00;
    insertOpExt = true;
  } else if(inst == instruction::PUSH) {
    if(isImm) {
      unsigned char opcode = 0x68;
      if(imm >= -128 && imm < 127) {
        opcode |= 0x02;
      }
      buf.push_back(opcode);
      opExt = 0x00;
      insertOpExt = true;
    } else {
      buf.push_back(0xFF);
      opExt = 0x06;
      insertOpExt = true;
    }

  } else if(inst == instruction::STMXCSR) {
    buf.push_back(0x0F);
    buf.push_back(0xAE);
    opExt = 0x03;
    insertOpExt = true;
  } else if(inst == instruction::FLDCW) {
    buf.push_back(0xD9);
    opExt = 0x05;
    insertOpExt = true;
  } else if(inst == instruction::FSTCW) {
    buf.push_back(0x9B);
    buf.push_back(0xD9);
    opExt = 0x07;
    insertOpExt = true;
  } else if(inst == instruction::FXCH) {
    buf.push_back(0xD9);
    opExt = 0x01;
    insertOpExt = true;
  } else if(inst == instruction::LDMXCSR) {
    buf.push_back(0x0F);
    buf.push_back(0xAE);
    opExt = 0x02;
    insertOpExt = true;
  } else if(inst == instruction::RETN) {
    /*Near return*/
    buf.push_back(0xC3);
    modrmBytePreCalculated = true;
  } else if(inst == instruction::RETF) {
    buf.push_back(0xCB);
    modrmBytePreCalculated = true;
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
    modrmByte |= ((static_cast<unsigned int>(rReg) & 0x07) << 3);
  }
  modrmByte |= static_cast<unsigned int>(rmReg) & 0x07;
  /*We add the modrmByte if it was not already added*/
  if(!modrmBytePreCalculated)
    buf.push_back(modrmByte);

  /*Now if rmReg is sp or r12 and the addressing mode is mem, we need to specify a SIB byte that
   evaluates as sp or r12*/
  if(addrMode == addressing_mode::MEM && (rmReg == reg::R4 || rmReg == reg::R12)) {
    unsigned char sib = 0x20;
    /*The first two bits (00) indicate a scale of 1.
     The next 3 (100) indicate that there is no index
     and the next 3 indicate the base register*/
    sib |= static_cast<unsigned int>(rmReg) & 0x07;
    buf.push_back(sib);
  }

  /*Now we encode the displacement bytes. If rmReg is r5 or r13 then we always
   encode a byte even when disp is zero*/
  if(addrMode == addressing_mode::MEM && (rmDisp != 0 || rmReg == reg::R5 || rmReg == reg::R13)) {
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
      if(imm >= -128 && imm <= 127 && inst != instruction::MOV) {
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
} // namespace dynsolver
