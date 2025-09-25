#pragma once

#include <cstdint>
#include <cstddef>


namespace Disassembler {

// Calls all necessary init functions for the maps
void initMaps();

// Returns a string that maps to the given opcode byte. Sets 'lengthOut' to strlen(returnValue)
// lengthOut == nullptr is valid, in case the length is not needed
const char* getOpcode(uint8_t opcode, size_t* lengthOut);

// Returns a string that maps to the given register byte. Sets 'lengthOut' to strlen(returnValue)
// lengthOut == nullptr is valid, in case the length is not needed
const char* getRegister(uint8_t reg, size_t* lengthOut);

}
