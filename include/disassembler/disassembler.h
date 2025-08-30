#pragma once

#include <cstdint>
#include <cstddef>


// Writes an opcode label into the writeBuffer based on the char code passed
size_t writeOpcode(char opcode, char* writeBuffer, size_t* writeBufferLength);

// Writes a register label into the writeBuffer based on the char code passed
size_t writeRegister(char reg, char* writeBuffer, size_t* writeBufferLength);

// Writes a memory reference label into the writeBuffer based on the designated chunk of memory in fileData
size_t writeMemory(char* fileData, size_t startIndex, char* writeBuffer, size_t* writeBufferLength);

// Writes an immediate value label into the writeBuffer based on the designated chunk of memory in fileData
size_t writeImmediate(char* fileData, size_t startIndex, char* writeBuffer, size_t* writeBufferLength);

// Writes the padding between labels based on what type of labels are nearby
void writePadding(char lastPrefix, char nextPrefix, char* writeBuffer, size_t* writeBufferLength);

// Top level function. Takes the data from fileNameIn and puts the disassembly into fileNameOut
void generateDisassemble(const char* fileNameIn, const char* fileNameOut);
