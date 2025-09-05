#include "assembler/assembler.h"

using namespace Assembler;


size_t Assembler::writeOpcode(uint8_t opcode, uint8_t* writeBuffer, size_t* writeBufferLength) {

    return 0;

}

size_t Assembler::writeRegister(uint8_t reg, uint8_t* writeBuffer, size_t* writeBufferLength) {

    return 1;

}

size_t Assembler::writeMemory(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength) {

    return 0;

}

size_t Assembler::writeImmediate(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength) {

    return 0;

}

void Assembler::writePadding(uint8_t lastPrefix, uint8_t nextPrefix, uint8_t* writeBuffer, size_t* writeBufferLength) {

    return;
    
}

void Assembler::generateAssemble(const char* fileNameIn, const char* fileNameOut) {

    return;

}
