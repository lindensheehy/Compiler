#pragma once

#include "assembler/types.h"
#include "core/File.h"

#include <cstddef>


namespace Assembler {

enum class SignatureOperandType : uint8_t {

    NONE,

    RM32,
    R32,
    IMM8,
    IMM16,
    IMM32,
    REL8,
    REL32,
    MEM,

    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    ESP,
    EBP,

};

enum class Immediate : uint8_t {
    NONE,
    ONE_BYTE,
    TWO_BYTES,
    FOUR_BYTES,
};

enum class Mode : uint8_t {

    NONE = 10,

    SLASH_0 = 0,
    SLASH_1 = 1,
    SLASH_2 = 2,
    SLASH_3 = 3,
    SLASH_4 = 4,
    SLASH_5 = 5,
    SLASH_6 = 6,
    SLASH_7 = 7,
    SLASH_R = 8,

    PLUS_RD = 9,
    
};

typedef uint8_t Preference;
typedef uint8_t OpcodeByte;
typedef uint8_t OpcodeBytesLength;

struct InstructionSignature {
    Preference preference;
    Opcode opcode;
    SignatureOperandType operands[3];
    OpcodeByte opcodeBytes[4];
    OpcodeBytesLength opcodeBytesLength;
    Mode mode;
    Immediate immediate;
};

constexpr InstructionSignature lookup[] = {

    /*
        Smaller 'preference' value is preferred
    */

    // ADD
    {
        Preference{2}, Opcode::ADD,
        {SignatureOperandType::RM32, SignatureOperandType::R32, SignatureOperandType::NONE},
        {0x01}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::ADD,
        {SignatureOperandType::R32, SignatureOperandType::RM32, SignatureOperandType::NONE},
        {0x03}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{4}, Opcode::ADD,
        {SignatureOperandType::RM32, SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x81}, OpcodeBytesLength{1},
        Mode::SLASH_0, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::ADD,
        {SignatureOperandType::RM32, SignatureOperandType::IMM8, SignatureOperandType::NONE},
        {0x83}, OpcodeBytesLength{1},
        Mode::SLASH_0, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::ADD,
        {SignatureOperandType::EAX, SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x05}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // SUB
    {
        Preference{2}, Opcode::SUB, 
        {SignatureOperandType::RM32, SignatureOperandType::R32, SignatureOperandType::NONE},
        {0x29}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::SUB, 
        {SignatureOperandType::R32, SignatureOperandType::RM32, SignatureOperandType::NONE},
        {0x2B}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{4}, Opcode::SUB, 
        {SignatureOperandType::RM32, SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x81}, OpcodeBytesLength{1},
        Mode::SLASH_5, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::SUB, 
        {SignatureOperandType::RM32, SignatureOperandType::IMM8, SignatureOperandType::NONE},
        {0x83}, OpcodeBytesLength{1},
        Mode::SLASH_5, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::SUB, 
        {SignatureOperandType::EAX, SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x2D}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // INC
    {
        Preference{1}, Opcode::INC,   
        {SignatureOperandType::R32,   SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x48}, OpcodeBytesLength{1},
        Mode::PLUS_RD, Immediate::NONE
    },
    {
        Preference{2}, Opcode::INC,   
        {SignatureOperandType::RM32,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xFF}, OpcodeBytesLength{1},
        Mode::SLASH_0, Immediate::NONE
    },

    // DEC
    {
        Preference{1}, Opcode::DEC,   
        {SignatureOperandType::R32,   SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x48}, OpcodeBytesLength{1},
        Mode::PLUS_RD, Immediate::NONE
    },
    {
        Preference{2}, Opcode::DEC,   
        {SignatureOperandType::RM32,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xFF}, OpcodeBytesLength{1},
        Mode::SLASH_1, Immediate::NONE
    },

    // IMUL
    {
        Preference{4}, Opcode::IMUL,  
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::NONE},
        {0x0F, 0xAF}, OpcodeBytesLength{2},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{3}, Opcode::IMUL,  
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::IMM32},
        {0x69}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::FOUR_BYTES
    },
    {
        Preference{2}, Opcode::IMUL,  
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::IMM8},
        {0x6B}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::IMUL,  
        {SignatureOperandType::RM32,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xF7}, OpcodeBytesLength{1},
        Mode::SLASH_5, Immediate::NONE
    },

    // IDIV
    {
        Preference{1}, Opcode::IDIV,  
        {SignatureOperandType::RM32,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xF7}, OpcodeBytesLength{1},
        Mode::SLASH_7, Immediate::NONE
    },

    // CDQ
    {
        Preference{1}, Opcode::CDQ,   
        {SignatureOperandType::NONE,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x99}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::NONE
    },

    // AND
    {
        Preference{2}, Opcode::AND,   
        {SignatureOperandType::RM32,  SignatureOperandType::R32,   SignatureOperandType::NONE},
        {0x21}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::AND,   
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::NONE},
        {0x23}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{4}, Opcode::AND,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x81}, OpcodeBytesLength{1},
        Mode::SLASH_4, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::AND,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM8,  SignatureOperandType::NONE},
        {0x83}, OpcodeBytesLength{1},
        Mode::SLASH_4, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::AND,   
        {SignatureOperandType::EAX,   SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x25}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // OR
    {
        Preference{2}, Opcode::OR,    
        {SignatureOperandType::RM32,  SignatureOperandType::R32,   SignatureOperandType::NONE},
        {0x09}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::OR,    
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::NONE},
        {0x0B}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{4}, Opcode::OR,    
        {SignatureOperandType::RM32,  SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x81}, OpcodeBytesLength{1},
        Mode::SLASH_1, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::OR,    
        {SignatureOperandType::RM32,  SignatureOperandType::IMM8,  SignatureOperandType::NONE},
        {0x83}, OpcodeBytesLength{1},
        Mode::SLASH_1, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::OR,    
        {SignatureOperandType::EAX,   SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x0D}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // XOR
    {
        Preference{2}, Opcode::XOR,   
        {SignatureOperandType::RM32,  SignatureOperandType::R32,   SignatureOperandType::NONE},
        {0x31}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::XOR,   
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::NONE},
        {0x33}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{4}, Opcode::XOR,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x81}, OpcodeBytesLength{1},
        Mode::SLASH_6, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::XOR,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM8,  SignatureOperandType::NONE},
        {0x83}, OpcodeBytesLength{1},
        Mode::SLASH_6, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::XOR,   
        {SignatureOperandType::EAX,   SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x35}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // CMP
    {
        Preference{2}, Opcode::CMP,   
        {SignatureOperandType::RM32,  SignatureOperandType::R32,   SignatureOperandType::NONE},
        {0x39}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::CMP,   
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::NONE},
        {0x3B}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{4}, Opcode::CMP,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x81}, OpcodeBytesLength{1},
        Mode::SLASH_7, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::CMP,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM8,  SignatureOperandType::NONE},
        {0x83}, OpcodeBytesLength{1},
        Mode::SLASH_7, Immediate::ONE_BYTE
    },
    {
        Preference{1}, Opcode::CMP,   
        {SignatureOperandType::EAX,   SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0x3D}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // TEST
    {
        Preference{2}, Opcode::TEST,  
        {SignatureOperandType::RM32,  SignatureOperandType::R32,   SignatureOperandType::NONE},
        {0x85}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{1}, Opcode::TEST,  
        {SignatureOperandType::EAX,   SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0xA9}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::TEST,  
        {SignatureOperandType::RM32,  SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0xF7}, OpcodeBytesLength{1},
        Mode::SLASH_0, Immediate::FOUR_BYTES
    },

    // JMP
    {
        Preference{1}, Opcode::JMP,   
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xEB}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JMP,   
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xE9}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // JE
    {
        Preference{1}, Opcode::JE,    
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x74}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JE,    
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x0F, 0x85}, OpcodeBytesLength{2},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // JNE
    {
        Preference{1}, Opcode::JNE,   
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x75}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JNE,   
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x0F, 0x85}, OpcodeBytesLength{2},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // JG
    {
        Preference{1}, Opcode::JG,    
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x7F}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JG,    
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x0F, 0x8F}, OpcodeBytesLength{2},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // JL
    {
        Preference{1}, Opcode::JL,    
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x7C}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JL,    
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x0F, 0x8C}, OpcodeBytesLength{2},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // JGE
    {
        Preference{1}, Opcode::JGE,   
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x7D}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JGE,   
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x0F, 0x8D}, OpcodeBytesLength{2},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // JLE
    {
        Preference{1}, Opcode::JLE,   
        {SignatureOperandType::REL8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x7E}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::JLE,   
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x0F, 0x8E}, OpcodeBytesLength{2},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // CALL
    {
        Preference{1}, Opcode::CALL,  
        {SignatureOperandType::REL32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xE8}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // RET
    {
        Preference{1}, Opcode::RET,   
        {SignatureOperandType::NONE,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xC3}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::NONE
    },
    {
        Preference{2}, Opcode::RET,   
        {SignatureOperandType::IMM16, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xC2}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::TWO_BYTES
    },

    // MOV
    {
        Preference{2}, Opcode::MOV,   
        {SignatureOperandType::RM32,  SignatureOperandType::R32,   SignatureOperandType::NONE},
        {0x89}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{2}, Opcode::MOV,   
        {SignatureOperandType::R32,   SignatureOperandType::RM32,  SignatureOperandType::NONE},
        {0x8B}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },
    {
        Preference{1}, Opcode::MOV,   
        {SignatureOperandType::R32,   SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0xB8}, OpcodeBytesLength{1},
        Mode::PLUS_RD, Immediate::FOUR_BYTES
    },
    {
        Preference{3}, Opcode::MOV,   
        {SignatureOperandType::RM32,  SignatureOperandType::IMM32, SignatureOperandType::NONE},
        {0xC7}, OpcodeBytesLength{1},
        Mode::SLASH_0, Immediate::FOUR_BYTES
    },

    // LEA
    {
        Preference{1}, Opcode::LEA,   
        {SignatureOperandType::R32,   SignatureOperandType::MEM,   SignatureOperandType::NONE},
        {0x8D}, OpcodeBytesLength{1},
        Mode::SLASH_R, Immediate::NONE
    },

    // PUSH
    {
        Preference{3}, Opcode::PUSH,  
        {SignatureOperandType::R32,   SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x50}, OpcodeBytesLength{1},
        Mode::PLUS_RD, Immediate::NONE
    },
    {
        Preference{4}, Opcode::PUSH,  
        {SignatureOperandType::RM32,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0xFF}, OpcodeBytesLength{1},
        Mode::SLASH_6, Immediate::NONE
    },
    {
        Preference{1}, Opcode::PUSH,  
        {SignatureOperandType::IMM8,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x6A}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::ONE_BYTE
    },
    {
        Preference{2}, Opcode::PUSH,  
        {SignatureOperandType::IMM32, SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x68}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::FOUR_BYTES
    },

    // POP
    {
        Preference{1}, Opcode::POP,   
        {SignatureOperandType::R32,   SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x58}, OpcodeBytesLength{1},
        Mode::PLUS_RD, Immediate::NONE
    },
    {
        Preference{2}, Opcode::POP,   
        {SignatureOperandType::RM32,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x8F}, OpcodeBytesLength{1},
        Mode::SLASH_0, Immediate::NONE
    },

    // NOP
    {
        Preference{1}, Opcode::NOP,   
        {SignatureOperandType::NONE,  SignatureOperandType::NONE,  SignatureOperandType::NONE},
        {0x90}, OpcodeBytesLength{1},
        Mode::NONE, Immediate::NONE
    }

};
constexpr size_t lookupLength = sizeof(lookup) / sizeof(InstructionSignature);

// Returns true if 'operand' falls into the category defined by 'signatureOperandType'
bool operandIs(const Operand& operand, SignatureOperandType signatureOperandType, File* log = nullptr);

// Returns a pointer to the InstructionSignature that best fits the Instruction. Returns nullptr if none match
const InstructionSignature* matchInstruction(const Instruction& instruction, File* log = nullptr);

}
