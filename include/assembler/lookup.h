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
    Opcode opcode;
    SignatureOperandType operands[3];
};

constexpr InstructionSignature lookup[] = {

    // ADD
    {Opcode::ADD,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::ADD,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::ADD,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::ADD,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {Opcode::ADD,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // SUB
    {Opcode::SUB,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::SUB,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::SUB,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::SUB,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {Opcode::SUB,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // INC
    {Opcode::INC,   {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::INC,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // DEC
    {Opcode::DEC,   {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::DEC,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // IMUL
    {Opcode::DEC,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::DEC,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::IMM32}},
    {Opcode::DEC,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::IMM8}},
    {Opcode::DEC,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // IDIV
    {Opcode::DEC,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // CDQ
    {Opcode::DEC,   {SignatureOperandType::NONE,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // AND
    {Opcode::AND,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::AND,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::AND,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::AND,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {Opcode::AND,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // OR
    {Opcode::OR,    {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::OR,    {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::OR,    {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::OR,    {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {Opcode::OR,    {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // XOR
    {Opcode::XOR,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::XOR,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::XOR,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::XOR,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {Opcode::XOR,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // CMP
    {Opcode::CMP,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::CMP,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::CMP,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::CMP,   {SignatureOperandType::RM32,      SignatureOperandType::IMM8,     SignatureOperandType::NONE}},
    {Opcode::CMP,   {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // TEST
    {Opcode::TEST,  {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::TEST,  {SignatureOperandType::EAX,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::TEST,  {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // JMP
    {Opcode::JMP,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JMP,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JE
    {Opcode::JE,    {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JE,    {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JNE
    {Opcode::JNE,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JNE,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JG
    {Opcode::JG,    {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JG,    {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JL
    {Opcode::JL,    {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JL,    {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JGE
    {Opcode::JGE,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JGE,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // JLE
    {Opcode::JLE,   {SignatureOperandType::REL8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::JLE,   {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // CALL
    {Opcode::CALL,  {SignatureOperandType::REL32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // RET
    {Opcode::RET,   {SignatureOperandType::NONE,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::RET,   {SignatureOperandType::IMM16,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // MOV
    {Opcode::MOV,   {SignatureOperandType::RM32,      SignatureOperandType::R32,      SignatureOperandType::NONE}},
    {Opcode::MOV,   {SignatureOperandType::R32,       SignatureOperandType::RM32,     SignatureOperandType::NONE}},
    {Opcode::MOV,   {SignatureOperandType::R32,       SignatureOperandType::IMM32,    SignatureOperandType::NONE}},
    {Opcode::MOV,   {SignatureOperandType::RM32,      SignatureOperandType::IMM32,    SignatureOperandType::NONE}},

    // LEA
    {Opcode::LEA,   {SignatureOperandType::R32,       SignatureOperandType::MEM,      SignatureOperandType::NONE}},

    // PUSH
    {Opcode::PUSH,  {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::PUSH,  {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::PUSH,  {SignatureOperandType::IMM8,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::PUSH,  {SignatureOperandType::IMM32,     SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // POP
    {Opcode::POP,   {SignatureOperandType::R32,       SignatureOperandType::NONE,     SignatureOperandType::NONE}},
    {Opcode::POP,   {SignatureOperandType::RM32,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

    // NOP
    {Opcode::NOP,   {SignatureOperandType::NONE,      SignatureOperandType::NONE,     SignatureOperandType::NONE}},

};

}
