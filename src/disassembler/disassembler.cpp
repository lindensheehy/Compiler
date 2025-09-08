#include "disassembler/disassembler.h"

#include "disassembler/map.h"
#include "core/fileio.h"

#include <string.h>
#include <cstdio>

using namespace Disassembler;


size_t Disassembler::writeOpcode(uint8_t opcode, uint8_t* writeBuffer, size_t* writeBufferLength) {

    size_t strLength;
    const char* str = getOpcode(opcode, &strLength);

    uint8_t* writePtr = writeBuffer + (*writeBufferLength);
    memcpy(writePtr, str, strLength);

    (*writeBufferLength) += strLength;

    return 1;

}

size_t Disassembler::writeRegister(uint8_t reg, uint8_t* writeBuffer, size_t* writeBufferLength) {
    
    size_t strLength;
    const char* str = getRegister(reg, &strLength);

    uint8_t* writePtr = writeBuffer + (*writeBufferLength);
    memcpy(writePtr, str, strLength);

    (*writeBufferLength) += strLength;

    return 1;

}

size_t Disassembler::writeMemory(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength) {
    
    writeBuffer[(*writeBufferLength)] = '[';
    (*writeBufferLength)++;

    writeRegister(fileData[startIndex], writeBuffer, writeBufferLength);

    uint8_t offsetLength = fileData[startIndex + 1];
    uint8_t offsetFirst = fileData[startIndex + 2];
    if (offsetLength != '1' || offsetFirst != 0x00) {

        static constexpr char padding[] = " + ";
        constexpr size_t paddingLen = sizeof(padding) - 1;

        memcpy(writeBuffer + (*writeBufferLength), padding, paddingLen);
        (*writeBufferLength) += paddingLen;

        writeImmediate(fileData, startIndex + 1, writeBuffer, writeBufferLength);

    }

    writeBuffer[(*writeBufferLength)] = ']';
    (*writeBufferLength)++;

    size_t consumed = 2;
    switch (offsetLength) {
        case '1':
            consumed += 1;
            break;
        case '4':
            consumed += 4;
            break;
        default:
            // log error case
            break;
    }

    return consumed;

}

size_t Disassembler::writeImmediate(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength) {

    size_t deltaIndex;
    int written;

    switch (fileData[startIndex]) {

        case '1': {

            uint8_t value = fileData[startIndex + 1];

            const size_t writeSize = 5; // ex. "0x12\0"
            written = std::snprintf(reinterpret_cast<char*>(writeBuffer), writeSize, "0x%X", value);

            deltaIndex = 2; // '1' and the byte value

            break;

        }

        case '4': {

            uint32_t value;
            memcpy(&value, fileData + startIndex + 1, sizeof(uint32_t));

            const size_t writeSize = 11; // ex. "0x12345678\0"
            written = std::snprintf(reinterpret_cast<char*>(writeBuffer), writeSize, "0x%X", value);

            deltaIndex = 5; // '4' and the 4 byte value

            break;

        }

        default: {
            // log error case
            break;
        }

    }

    (*writeBufferLength) += written;

    return deltaIndex;

}

void Disassembler::writePadding(uint8_t lastPrefix, uint8_t nextPrefix, uint8_t* writeBuffer, size_t* writeBufferLength) {

    /*
        There are three types of padding:
        1. A newline "\r\n". This goes before opcodes
        2. A single space ' '. This goes after opcodes
        3. A comma and space ", ". This goes between operands (when the above two cases dont apply)
    */

    static constexpr char newLinePadding[] = "\r\n";
    constexpr size_t newLinePaddingLen = sizeof(newLinePadding) - 1;

    static constexpr char spacePadding[] = " ";
    constexpr size_t spacePaddingLen = sizeof(spacePadding) - 1;

    static constexpr char commaSpacePadding[] = ", ";
    constexpr size_t commaSpacePaddingLen = sizeof(commaSpacePadding) - 1;

    struct {
        const char* s;
        size_t len;
    } padding;

    // Case 1
    if (nextPrefix == 'O') {
        padding.s = newLinePadding;
        padding.len = newLinePaddingLen;
    }

    // Case 2
    else if (lastPrefix == 'O') {
        padding.s = spacePadding;
        padding.len = spacePaddingLen;
    }

    // Case 3
    else {
        padding.s = commaSpacePadding;
        padding.len = commaSpacePaddingLen;
    }

    uint8_t* writeBufferPointer = writeBuffer + (*writeBufferLength);
    memcpy(writeBufferPointer, padding.s, padding.len);

    (*writeBufferLength) += padding.len;

    return;
    
}

ErrorCode Disassembler::generateDisassemble(const char* fileNameIn, const char* fileNameOut) {

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

    {
        // First, write the file name as a comment at the top
        static constexpr char fileNamePrefix[] = "; File: ";
        constexpr size_t fileNamePrefixLen = sizeof(fileNamePrefix) - 1;

        memcpy(writeBuffer, fileNamePrefix, fileNamePrefixLen);
        writeBufferLength += fileNamePrefixLen;

        size_t fileNameInLen = strlen(fileNameIn);
        memcpy(writeBuffer + writeBufferLength, fileNameIn, fileNameInLen);
        writeBufferLength += fileNameInLen;
    }

    initMaps();

    for (size_t i = 0; i < fileLength; i++) {

        writePadding(lastPrefix, file[i], writeBuffer, &writeBufferLength);

        switch (file[i]) {
            
            // Opcode
            case 'O': {

                bytesConsumed = writeOpcode(file[i + 1], writeBuffer, &writeBufferLength);
                i += bytesConsumed;

                break;

            }

            // Register
            case 'R': {

                bytesConsumed = writeRegister(file[i + 1], writeBuffer, &writeBufferLength);
                i += bytesConsumed;
                
                break;
                
            }

            // Memory
            case 'M': {

                bytesConsumed = writeMemory(file, i + 1, writeBuffer, &writeBufferLength);
                i += bytesConsumed;
                
                break;
                
            }

            // Immediate
            case 'I': {

                bytesConsumed = writeImmediate(file, i + 1, writeBuffer, &writeBufferLength);
                i += bytesConsumed;
                
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
