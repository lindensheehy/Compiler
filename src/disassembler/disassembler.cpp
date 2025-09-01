#include "disassembler/disassembler.h"

#include "disassembler/map.h"
#include "core/fileio.h"

#include <string.h>
#include <cstdio>


size_t writeOpcode(char opcode, char* writeBuffer, size_t* writeBufferLength) {

    size_t strLength;
    const char* str = getOpcode(static_cast<uint8_t>(opcode), &strLength);

    char* writePtr = writeBuffer + (*writeBufferLength);
    memcpy(writePtr, str, strLength);

    (*writeBufferLength) += strLength;

    return 1;

}

size_t writeRegister(char reg, char* writeBuffer, size_t* writeBufferLength) {
    
    size_t strLength;
    const char* str = getRegister(static_cast<uint8_t>(reg), &strLength);

    char* writePtr = writeBuffer + (*writeBufferLength);
    memcpy(writePtr, str, strLength);

    (*writeBufferLength) += strLength;

    return 1;

}

size_t writeMemory(char* fileData, size_t startIndex, char* writeBuffer, size_t* writeBufferLength) {
    
    writeBuffer[(*writeBufferLength)] = '[';
    (*writeBufferLength)++;

    writeRegister(fileData[startIndex], writeBuffer, writeBufferLength);

    char offsetLength = fileData[startIndex + 1];
    char offsetFirst = fileData[startIndex + 2];
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

size_t writeImmediate(char* fileData, size_t startIndex, char* writeBuffer, size_t* writeBufferLength) {

    size_t deltaIndex;
    int written;

    switch (fileData[startIndex]) {

        case '1': {

            uint8_t value = fileData[startIndex + 1];

            const size_t writeSize = 5; // ex. "0x12\0"
            written = std::snprintf(writeBuffer, writeSize, "0x%X", value);

            deltaIndex = 2; // '1' and the byte value

            break;

        }

        case '4': {

            uint32_t value;
            memcpy(&value, fileData + startIndex + 1, sizeof(uint32_t));

            const size_t writeSize = 11; // ex. "0x12345678\0"
            written = std::snprintf(writeBuffer, writeSize, "0x%X", value);

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

void writePadding(char lastPrefix, char nextPrefix, char* writeBuffer, size_t* writeBufferLength) {

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

    char* writeBufferPointer = writeBuffer + (*writeBufferLength);
    memcpy(writeBufferPointer, padding.s, padding.len);

    (*writeBufferLength) += padding.len;

    return;
    
}

void generateDisassemble(const char* fileNameIn, const char* fileNameOut) {

    char* file = readFile(fileNameIn);
    size_t fileLength;
    bool file_length_rc = getFileLength(fileNameIn, &fileLength);
    if(!file_length_rc){
        return;
    }
    constexpr size_t WRITE_BUFFER_SIZE = 65536;
    char* writeBuffer = new char[WRITE_BUFFER_SIZE] {0x00};
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
                // log error case
                return;
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

    return;

}
