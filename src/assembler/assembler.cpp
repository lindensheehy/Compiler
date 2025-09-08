#include "assembler/assembler.h"

#include "core/fileio.h"

#include <cstdio>
#include <string.h>

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

ErrorCode Assembler::generateAssemble(const char* fileNameIn, const char* fileNameOut) {

    uint8_t* file = readFile(fileNameIn);
    size_t fileLength;
    bool file_length_rc = getFileLength(fileNameIn, &fileLength);
    if(!file_length_rc){
        return ErrorCode::INVALID_FILE;
    }
    constexpr size_t WRITE_BUFFER_SIZE = 65536;
    uint8_t* writeBuffer = new uint8_t[WRITE_BUFFER_SIZE] {0x00};
    size_t writeBufferLength = 0;
    constexpr size_t WRITE_BUFFER_LIMIT = WRITE_BUFFER_SIZE - 64;

    char lastPrefix = 0;
    size_t bytesConsumed;

    // initMaps();

    for (size_t i = 0; i < fileLength; i++) {

        switch (file[i]) {
            
            // Opcode
            case 'O': {

                // bytesConsumed = writeOpcode(file[i + 1], writeBuffer, &writeBufferLength);
                // i += bytesConsumed;

                break;

            }

            // Register
            case 'R': {

                // bytesConsumed = writeRegister(file[i + 1], writeBuffer, &writeBufferLength);
                // i += bytesConsumed;
                
                break;
                
            }

            // Memory
            case 'M': {

                // bytesConsumed = writeMemory(file, i + 1, writeBuffer, &writeBufferLength);
                // i += bytesConsumed;
                
                break;
                
            }

            // Immediate
            case 'I': {

                // bytesConsumed = writeImmediate(file, i + 1, writeBuffer, &writeBufferLength);
                // i += bytesConsumed;
                
                break;
                
            }

            default: {
                return ErrorCode::FOUND_INVALID_PREFIX;
            }

        }

        if (writeBufferLength > WRITE_BUFFER_LIMIT) {
            writeFile(fileNameOut, writeBuffer, writeBufferLength);
            memset(writeBuffer, 0x00, WRITE_BUFFER_SIZE);
            writeBufferLength = 0;
        }

        lastPrefix = file[i];

    }

    writeFile(fileNameOut, writeBuffer, writeBufferLength);
    delete[] writeBuffer;
    delete[] file;

    return ErrorCode::NONE;

}
