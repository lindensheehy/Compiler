
const char* opcodeMap[] = {
    "add" // 0x00
};


void writeOpcode(char opcode, char* writeBuffer, int* writeBufferLength) {

    switch (opcode) {

        case 0x00: {
            writeBuffer[(*writeBufferLength)]     = 'a';
            writeBuffer[(*writeBufferLength) + 1] = 'd';
            writeBuffer[(*writeBufferLength) + 2] = 'd';
            writeBuffer[(*writeBufferLength) + 3] = ' ';
            (*writeBufferLength) += 4;
            break;
        }

        case 0x01: {
            writeBuffer[(*writeBufferLength)]     = 's';
            writeBuffer[(*writeBufferLength) + 1] = 'u';
            writeBuffer[(*writeBufferLength) + 2] = 'b';
            writeBuffer[(*writeBufferLength) + 3] = ' ';
            (*writeBufferLength) += 4;
            break;
        }

        // etc

    }

    return;

}

void writeRegister(char reg, char* writeBuffer, int* writeBufferLength) {
    return;
}

void writeMemory(int startIndex, char* writeBuffer, int* writeBufferLength) {
    return;
}

void writeIntermediate(int startIndex, char* writeBuffer, int* writeBufferLength) {
    return;
}

void generateDisassemble(const char* fileNameIn, const char* fileNameOut) {

    char* file = readFile(fileNameIn);
    int fileLength = getFileLength(fileNameIn);

    constexpr int bufferSize = 2048;
    char* writeBuffer = new char[bufferSize];
    int writeBufferLength = 0;
    const int writeBufferLimit = bufferSize - 64;

    for (int i = 0; i < fileLength; i++) {

        switch (file[i]) {

            case 'O': {

                writeOpcode(file[i + 1], writeBuffer, &writeBufferLength);
                i += 1;

                break;

            }

            case 'R': {

                writeRegister(file[i + 1], writeBuffer, &writeBufferLength);
                i += 1;
                
                break;
                
            }

            case 'M': {

                writeMemory(i + 1, writeBuffer, &writeBufferLength);
                i += 1; // this would need to be dynamic
                
                break;
                
            }

            case 'I': {

                writeIntermediate(i + 1, writeBuffer, &writeBufferLength);
                i += 1; // this would need to be dynamic
                
                break;
                
            }

            default: {
                std::cout << "Failed to disassemble! Reason: Found invalid token prefix char(" << int(file[i]) << ")" << std::endl;
                return;
            }

        }

        if (writeBufferLength > writeBufferLimit) {
            writeFile(fileNameOut, writeBuffer, writeBufferLength);
            memset(writeBuffer, 0x00, bufferSize);
            writeBufferLength = 0;
        }

    }

    return;

}
