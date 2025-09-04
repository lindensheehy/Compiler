#pragma once

#include <cstddef>
#include <cstdint>


// Reads from fileName and returns a new heap allocated buffer containing the contents
uint8_t* readFile(const char* fileName);

// Returns the length of fileName
bool getFileLength(const char* fileName, uintmax_t* fileLengthOut);

// Appends the contents of writeBuffer to fileNameOut. Will write exactly writeBufferLength bytes.
void writeFile(const char* fileName, uint8_t* writeBuffer, size_t writeBufferLength);
