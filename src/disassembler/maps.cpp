#include "../../include/disassembler/maps.h"

constexpr size_t BYTE_RANGE = 256;
const char* opcodeMap[BYTE_RANGE];
const char* registerMap[BYTE_RANGE];

void initOpcodeMap() {
    
    // Set all to "???" to make unmapped outputs more readable
    const char* unknown = "???";
    for (size_t i = 0; i < BYTE_RANGE; i++) {
        opcodeMap[i] = unknown;
    }

    // Currently mapped opcodes
    opcodeMap[0x00] = "add";
    opcodeMap[0x01] = "sub";
    opcodeMap[0x02] = "inc";
    opcodeMap[0x03] = "dec";
    opcodeMap[0x04] = "imul";
    opcodeMap[0x05] = "idiv";
    opcodeMap[0x06] = "cdq";
    opcodeMap[0x07] = "and";
    opcodeMap[0x08] = "or";
    opcodeMap[0x09] = "xor";
    opcodeMap[0x0A] = "cmp";
    opcodeMap[0x0B] = "test";
    opcodeMap[0x0C] = "jmp";
    opcodeMap[0x0D] = "je";
    opcodeMap[0x0E] = "jne";
    opcodeMap[0x0F] = "jg";
    opcodeMap[0x10] = "jl";
    opcodeMap[0x11] = "jge";
    opcodeMap[0x12] = "jle";
    opcodeMap[0x13] = "call";
    opcodeMap[0x14] = "ret";
    opcodeMap[0x15] = "mov";
    opcodeMap[0x16] = "lea";
    opcodeMap[0x17] = "push";
    opcodeMap[0x18] = "pop";
    opcodeMap[0x19] = "nop";

    return;

}

void initRegisterMap() {

    // Set all to "???" to make unmapped outputs more readable
    const char* unknown = "???";
    for (size_t i = 0; i < BYTE_RANGE; i++) {
        registerMap[i] = unknown;
    }

    // Currently mapped registers
    registerMap[0x00] = "eax";
    registerMap[0x01] = "ebx";
    registerMap[0x02] = "ecx";
    registerMap[0x03] = "edx";
    registerMap[0x04] = "esi";
    registerMap[0x05] = "edi";
    registerMap[0x06] = "esp";
    registerMap[0x07] = "ebp";

    return;

}

void initMaps() {

    initOpcodeMap();
    initRegisterMap();

    return;

}

const char* getOpcode(uint8_t opcode, size_t* lengthOut) {
    const char* ret = opcodeMap[opcode];
    if (lengthOut) (*lengthOut) = strlen(ret);
    return ret;
}

const char* getRegister(uint8_t reg, size_t* lengthOut) {
    const char* ret = registerMap[reg];
    if (lengthOut) (*lengthOut) = strlen(ret);
    return ret;
}
