#include "disassembler/disassembler.h"

#include "disassembler/map.h"
#include "core/fileio.h"
#include "core/File.h"

#include <string.h>
#include <cstddef>
#include <cstdio>

using namespace Disassembler;

// Forward declaration
size_t writeImmediate(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength, File* log);


// Writes an opcode label into the writeBuffer based on the char code passed
size_t writeOpcode(uint8_t opcode, uint8_t* writeBuffer, size_t* writeBufferLength, File* log) {

    size_t strLength;
    const char* str = getOpcode(opcode, &strLength);

    uint8_t* writePtr = writeBuffer + (*writeBufferLength);
    memcpy(writePtr, str, strLength);

    (*writeBufferLength) += strLength;

    return 1;

}

// Writes a register label into the writeBuffer based on the char code passed
size_t writeRegister(uint8_t reg, uint8_t* writeBuffer, size_t* writeBufferLength, File* log) {

    size_t strLength;
    const char* str = getRegister(reg, &strLength);

    uint8_t* writePtr = writeBuffer + (*writeBufferLength);
    memcpy(writePtr, str, strLength);

    (*writeBufferLength) += strLength;

    return 1;

}

// Writes a memory reference label into the writeBuffer based on the designated chunk of memory in fileData
size_t writeMemory(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength, File* log) {
    
    writeBuffer[(*writeBufferLength)] = '[';
    (*writeBufferLength)++;

    writeRegister(fileData[startIndex], writeBuffer, writeBufferLength, log);

    uint8_t offsetLength = fileData[startIndex + 1];
    uint8_t offsetFirst = fileData[startIndex + 2];
    if (offsetLength != '1' || offsetFirst != 0x00) {

        static constexpr char padding[] = " + ";
        constexpr size_t paddingLen = sizeof(padding) - 1;

        memcpy(writeBuffer + (*writeBufferLength), padding, paddingLen);
        (*writeBufferLength) += paddingLen;

        writeImmediate(fileData, startIndex + 1, writeBuffer, writeBufferLength, log);

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

        default: {

            log->write("Error: Found an invalid memory offset length character!", true);
            log->write(" >> Found character \'");
            log->write(static_cast<char>(offsetLength));
            log->write("\' (int value ");
            log->write(static_cast<char>(offsetLength), false, WriteFormat::DECIMAL);
            log->write("), which does not match any known length character.", true);

            break;

        }

    }

    return consumed;

}

// Writes an immediate value label into the writeBuffer based on the designated chunk of memory in fileData
size_t writeImmediate(uint8_t* fileData, size_t startIndex, uint8_t* writeBuffer, size_t* writeBufferLength, File* log) {

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
            
            log->write("Error: Found an invalid immediate length character!", true);
            log->write(" >> Found character \'");
            log->write(static_cast<char>(fileData[startIndex]));
            log->write("\' (int value ");
            log->write(static_cast<char>(fileData[startIndex]), false, WriteFormat::DECIMAL);
            log->write("), which does not match any known length character.", true);

            break;

        }

    }

    (*writeBufferLength) += written;

    return deltaIndex;

}

// Writes the padding between labels based on what type of labels are nearby
void writePadding(uint8_t lastPrefix, uint8_t nextPrefix, uint8_t* writeBuffer, size_t* writeBufferLength, File* log) {

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

    File log("logs/dissassembler.log");
    log.clear();

    size_t fileLength;
    uint8_t* file;

    // The scope here lets fileIn be quickly destructed, since its an OS resource
    {
        File fileIn(fileNameIn);
        fileLength = fileIn.getSize();
        file = fileIn.read();
    }

    if (fileLength == 0) {

        log.write("Error: The file at 'fileNameIn' (\"");
        log.write(fileNameIn);
        log.write("\") is empty!", true);

        return ErrorCode::INVALID_FILE;

    }

    if (file == nullptr) {

        log.write("Error: Failed to read from 'fileNameIn' (\"");
        log.write(fileNameIn);
        log.write("\")!", true);

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

        writePadding(lastPrefix, file[i], writeBuffer, &writeBufferLength, &log);

        switch (file[i]) {
            
            // Opcode
            case 'O': {
                bytesConsumed = writeOpcode(file[i + 1], writeBuffer, &writeBufferLength, &log);
                break;
            }

            // Register
            case 'R': {
                bytesConsumed = writeRegister(file[i + 1], writeBuffer, &writeBufferLength, &log);
                break;
            }

            // Memory
            case 'M': {
                bytesConsumed = writeMemory(file, i + 1, writeBuffer, &writeBufferLength, &log);
                break;
            }

            // Immediate
            case 'I': {
                bytesConsumed = writeImmediate(file, i + 1, writeBuffer, &writeBufferLength, &log);
                break;
            }

            default: {

                log.write("Error: Found an invalid prefix character!", true);
                log.write(" >> Found character \'");
                log.write(static_cast<char>(file[i]));
                log.write("\' (int value ");
                log.write(static_cast<char>(file[i]), false, WriteFormat::DECIMAL);
                log.write("), which does not match any known prefix.", true);

                return ErrorCode::FOUND_INVALID_PREFIX;

            }

        }

        i += bytesConsumed;

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
