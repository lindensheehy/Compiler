#pragma once

#include "assembler/data.h"

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

struct InstructionSignature {
    uint8_t preference;
    Opcode opcode;
    SignatureOperandType operands[3];
};

constexpr size_t lookupLength = sizeof(lookup) / sizeof(InstructionSignature);
constexpr InstructionSignature lookup[] = {

    // ADD
    {2, Opcode::ADD,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::ADD,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {4, Opcode::ADD,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::ADD,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {1, Opcode::ADD,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // SUB
    {2, Opcode::SUB,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::SUB,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {4, Opcode::SUB,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::SUB,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {1, Opcode::SUB,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // INC
    {1, Opcode::INC,   {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::INC,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // DEC
    {1, Opcode::DEC,   {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::DEC,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // IMUL
    {4, Opcode::IMUL,  {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {3, Opcode::IMUL,  {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::IMM32}},
    {2, Opcode::IMUL,  {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::IMM8}},
    {1, Opcode::IMUL,  {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // IDIV
    {1, Opcode::IDIV,  {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // CDQ
    {1, Opcode::CDQ,   {SignatureOperandType::NONE,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // AND
    {2, Opcode::AND,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::AND,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {4, Opcode::AND,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::AND,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {1, Opcode::AND,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // OR
    {2, Opcode::OR,    {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::OR,    {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {4, Opcode::OR,    {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::OR,    {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {1, Opcode::OR,    {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // XOR
    {2, Opcode::XOR,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::XOR,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {4, Opcode::XOR,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::XOR,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {1, Opcode::XOR,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // CMP
    {2, Opcode::CMP,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::CMP,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {4, Opcode::CMP,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::CMP,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {1, Opcode::CMP,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // TEST
    {2, Opcode::TEST,  {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {1, Opcode::TEST,  {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::TEST,  {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // JMP
    {1, Opcode::JMP,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JMP,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JE
    {1, Opcode::JE,    {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JE,    {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JNE
    {1, Opcode::JNE,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JNE,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JG
    {1, Opcode::JG,    {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JG,    {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JL
    {1, Opcode::JL,    {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JL,    {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JGE
    {1, Opcode::JGE,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JGE,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JLE
    {1, Opcode::JLE,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::JLE,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // CALL
    {1, Opcode::CALL,  {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // RET
    {1, Opcode::RET,   {SignatureOperandType::NONE,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::RET,   {SignatureOperandType::IMM16,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // MOV
    {2, Opcode::MOV,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {2, Opcode::MOV,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {1, Opcode::MOV,   {SignatureOperandType::R32,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {3, Opcode::MOV,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // LEA
    {1, Opcode::LEA,   {SignatureOperandType::R32,       SignatureOperandType::MEM,      SignatureOperandType::NONE}},

    // PUSH
    {3, Opcode::PUSH,  {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {4, Opcode::PUSH,  {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {1, Opcode::PUSH,  {SignatureOperandType::IMM8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::PUSH,  {SignatureOperandType::IMM32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // POP
    {1, Opcode::POP,   {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {2, Opcode::POP,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // NOP
    {1, Opcode::NOP,   {SignatureOperandType::NONE,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

};

bool operandIs(const Operand& operand, SignatureOperandType signatureOperandType);

}
