#pragma once

#include <cstdint>
#include <cstddef>


namespace Assembler {

// Writes an opcode label into the writeBuffer based on the char code passed
size_t writeOpcode(uint8_t opcode, uint8_t* writeBuffer, size_t* writeBufferLength);

// Writes a register label into the writeBuffer based on the char code passed
size_t writeRegister(uint8_t reg, uint8_t* writeBuffer, size_t* writeBufferLength);

// Writes a memory reference label into the writeBuffer based on the designated chunk of memory in fileData
size_t writeMemory(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength);

// Writes an immediate value label into the writeBuffer based on the designated chunk of memory in fileData
size_t writeImmediate(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength);

// Writes the padding between labels based on what type of labels are nearby
void writePadding(uint8_t lastPrefix, uint8_t nextPrefix, uint8_t* writeBuffer, size_t* writeBufferLength);

// Top level function. Takes the data from fileNameIn and puts the disassembly into fileNameOut
void generateAssemble(const char* fileNameIn, const char* fileNameOut);

}
