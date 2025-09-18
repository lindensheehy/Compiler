#pragma once

#include "assembler/data.h"

#include <cstdint>
#include <cstddef>


namespace Assembler {

enum class ErrorCode : uint32_t {
    NONE = 0,
    INVALID_FILE = 1,
    FOUND_INVALID_PREFIX = 2,
};

// Writes an opcode label into the writeBuffer based on the char code passed
size_t handleOpcode(uint8_t opcode, Instruction* instruction);

// Writes a register label into the writeBuffer based on the char code passed
size_t handleRegister(uint8_t reg, Instruction* instruction);

// Writes a memory reference label into the writeBuffer based on the designated chunk of memory in fileData
size_t handleMemory(uint8_t* fileData, size_t startIndex, Instruction* instruction);

// Writes an immediate value label into the writeBuffer based on the designated chunk of memory in fileData
size_t handleImmediate(uint8_t* fileData, size_t startIndex, Instruction* instruction);

// Writes the padding between labels based on what type of labels are nearby
void assembleInstruction(const Instruction& instruction, InstructionBytes* instructionBytesOut);

// Writes the IntructionBytes into the writeBuffer. Conditionally writes parts depending on the contents
void writeInstruction(const InstructionBytes& instructionBytes, uint8_t* writeBuffer, size_t* writeBufferLength);

// Top level function. Takes the data from fileNameIn and puts the disassembly into fileNameOut
ErrorCode generateAssemble(const char* fileNameIn, const char* fileNameOut);

}
