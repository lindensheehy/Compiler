#pragma once

#include <cstdint>


namespace Assembler {

enum class Opcode : uint8_t {

    ADD = 0x00,
    SUB = 0x01,
    INC = 0x02,
    DEC = 0x03,
    IMUL = 0x04,
    IDIV = 0x05,
    CDQ = 0x06,
    AND = 0x07,
    OR = 0x08,
    XOR = 0x09,
    CMP = 0x0A,
    TEST = 0x0B,
    JMP = 0x0C,
    JE = 0x0D,
    JNE = 0x0E,
    JG = 0x0F,
    JL = 0x10,
    JGE = 0x11,
    JLE = 0x12,
    CALL = 0x13,
    RET = 0x14,
    MOV = 0x15,
    LEA = 0x16,
    PUSH = 0x17,
    POP = 0x18,
    NOP = 0x19,

};

enum class Register : uint8_t {

    EAX = 0x00,
    ECX = 0x01,
    EDX = 0x02,
    EBX = 0x03,
    ESP = 0x04,
    EBP = 0x05,
    ESI = 0x06,
    EDI = 0x07,

};

enum class OperandType {
    REGISTER,
    IMMEDIATE,
    MEMORY,
};

struct Operand {
    
    OperandType type;
    
    union {

        Register reg;

        struct {
            uint8_t size;
            uint32_t value;
        } immediate;

        struct {
            Register reg;
            uint32_t offset;
        } mem;

    };

};

struct Instruction {

    Opcode opcode;

    Operand operands[3];

};

struct InstructionBytes {

    public:
        uint8_t opcode[4];
        uint8_t modrm;
    private:
        uint8_t padding[3];
    public:
        uint32_t displacement;
        uint32_t immediate;

};

}
