#pragma once

#include <cstddef>


// Reads from fileName and returns a new heap allocated buffer containing the contents
char* readFile(const char* fileName);

// Returns the length of fileName
size_t getFileLength(const char* fileName);

// Appends the contents of writeBuffer to fileNameOut. Will write exactly writeBufferLength bytes.
void writeFile(const char* fileName, const char* writeBuffer, size_t writeBufferLength);
