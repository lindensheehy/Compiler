#pragma once

#include <cstdint>  // for uint8_t
#include <cstddef>  // for size_t
#include <string.h> // for strlen

// Sets up the opcode map. This must be called before using getOpcode()
void initOpcodeMap();

// Sets up the register map. This must be called before using getRegister()
void initRegisterMap();

// Calls all necessary init functions for the maps
void initMaps();

// Returns a string that maps to the given opcode byte. Sets 'lengthOut' to strlen(returnValue)
// lengthOut == nullptr is valid, in case the length is not needed
const char* getOpcode(uint8_t opcode, size_t* lengthOut);

// Returns a string that maps to the given register byte. Sets 'lengthOut' to strlen(returnValue)
// lengthOut == nullptr is valid, in case the length is not needed
const char* getRegister(uint8_t reg, size_t* lengthOut);
