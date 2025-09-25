#include "assembler/assembler.h"

#include "assembler/lookup.h"
#include "core/fileio.h"

#include <cstdio>
#include <string.h>

using namespace Assembler;


// Writes an opcode label into 'instruction'
size_t handleOpcode(uint8_t opcode, Instruction* instruction) {

    instruction->opcode = static_cast<Opcode>(opcode);

    return 1;

}

// Writes a register label into 'instruction'
size_t handleRegister(uint8_t reg, Instruction* instruction) {

    size_t insertionIndex = instruction->operandCount;

    Operand* operand = &(instruction->operands[insertionIndex]);
    operand->type = OperandType::REGISTER;
    operand->reg = static_cast<Register>(reg);

    instruction->operandCount++;

    return 1;

}

// Writes a memory reference label into 'instruction'
size_t handleMemory(uint8_t* fileData, size_t startIndex, Instruction* instruction) {

    size_t insertionIndex = instruction->operandCount;

    Operand* operand = &(instruction->operands[insertionIndex]);
    operand->type = OperandType::MEMORY;
    operand->mem.reg = static_cast<Register>(fileData[startIndex]);

    size_t consumed = 0;

    switch (fileData[startIndex + 1]) {

        case '1': {

            operand->mem.offset = static_cast<uint32_t>(fileData[startIndex + 2]);

            consumed = 3;

            break;
        
        }

        case '4': {
            
            uint32_t bigEndianImm;
            memcpy(&bigEndianImm, fileData + startIndex + 2, sizeof(uint32_t));

            uint32_t littleEndianImm = 0;
            littleEndianImm |= (bigEndianImm << 24) & 0xFF000000;
            littleEndianImm |= (bigEndianImm << 8)  & 0x00FF0000;
            littleEndianImm |= (bigEndianImm >> 8)  & 0x0000FF00;
            littleEndianImm |= (bigEndianImm >> 24) & 0x000000FF;

            operand->mem.offset = littleEndianImm;

            consumed = 6;

            break;
        }

        default: {
            // log error case
            break;
        }

    }

    instruction->operandCount++;

    return consumed;

}

// Writes an immediate value label into 'instruction'
size_t handleImmediate(uint8_t* fileData, size_t startIndex, Instruction* instruction) {

    size_t insertionIndex = instruction->operandCount;

    Operand* operand = &(instruction->operands[insertionIndex]);
    operand->type = OperandType::IMMEDIATE;

    size_t consumed = 0;

    switch (fileData[startIndex]) {

        case '1': {

            operand->immediate.size = 1;
            operand->immediate.value = static_cast<uint32_t>(fileData[startIndex + 1]);

            consumed = 2;

            break;
        
        }

        case '4': {

            operand->immediate.size = 4;
            
            uint32_t bigEndianImm;
            memcpy(&bigEndianImm, fileData + startIndex + 1, sizeof(uint32_t));

            uint32_t littleEndianImm = 0;
            littleEndianImm |= (bigEndianImm << 24) & 0xFF000000;
            littleEndianImm |= (bigEndianImm << 8)  & 0x00FF0000;
            littleEndianImm |= (bigEndianImm >> 8)  & 0x0000FF00;
            littleEndianImm |= (bigEndianImm >> 24) & 0x000000FF;

            operand->immediate.value = littleEndianImm;

            consumed = 5;

            break;
        }

        default: {
            // log error case
            break;
        }

    }

    instruction->operandCount++;

    return consumed;

}

// Returns a pointer to the InstructionSignature that best fits the Instruction. Returns nullptr if none match
const InstructionSignature* matchInstruction(const Instruction& instruction) {

    const InstructionSignature* matchFound = nullptr;

    bool successfulMatch;

    for (size_t i = 0; i < lookupLength; i++) {

        if (instruction.opcode != lookup[i].opcode) continue;

        successfulMatch = true;

        for (size_t j = 0; j < 3; j++) {

            if (j >= instruction.operandCount) {
                if (lookup[i].operands[j] != SignatureOperandType::NONE) {
                    successfulMatch = false;
                    break;
                }
                continue;
            }

            if (!operandIs(instruction.operands[j], lookup[i].operands[j])) {
                successfulMatch = false;
                break;
            }

        }

        if (successfulMatch) {
            if (matchFound == nullptr || lookup[i].preference < matchFound->preference) {
                matchFound = &(lookup[i]);
                if (matchFound->preference == 1) return matchFound;
            }
        }

    }

    return matchFound;

}

// Assembles the instruction data from 'instruction' into 'instructionBytes'. Returns true on a failiure
bool assembleInstruction(const Instruction& instruction, InstructionBytes* instructionBytesOut) {

    const InstructionSignature* match = matchInstruction(instruction);
    if (match == nullptr) return true;

    return false;

}

// Writes the IntructionBytes into the writeBuffer. Conditionally writes parts depending on the contents
void writeInstruction(const InstructionBytes& instructionBytes, uint8_t* writeBuffer, size_t* writeBufferLength) {

    return;

}

ErrorCode Assembler::generateAssemble(const char* fileNameIn, const char* fileNameOut) {

    uint8_t* file = readFile(fileNameIn);
    size_t fileLength;
    bool file_length_rc = getFileLength(fileNameIn, &fileLength);
    if (!file_length_rc) {
        return ErrorCode::INVALID_FILE;
    }
    constexpr size_t WRITE_BUFFER_SIZE = 65536;
    uint8_t* writeBuffer = new uint8_t[WRITE_BUFFER_SIZE] {0x00};
    size_t writeBufferLength = 0;
    constexpr size_t WRITE_BUFFER_LIMIT = WRITE_BUFFER_SIZE - 64;

    size_t bytesConsumed;
    Instruction instructionBuilder {};
    InstructionBytes instructionBytes {};

    // initMaps();

    for (size_t i = 0; i < fileLength; i++) {

        switch (file[i]) {
            
            // Opcode
            case 'O': {

                assembleInstruction(instructionBuilder, &instructionBytes);
                writeInstruction(instructionBytes, writeBuffer, &writeBufferLength);

                bytesConsumed = handleOpcode(file[i + 1], &instructionBuilder);
                i += bytesConsumed;

                break;

            }

            // Register
            case 'R': {

                bytesConsumed = handleRegister(file[i + 1], &instructionBuilder);
                i += bytesConsumed;
                
                break;
                
            }

            // Memory
            case 'M': {

                bytesConsumed = handleMemory(file, i + 1, &instructionBuilder);
                i += bytesConsumed;
                
                break;
                
            }

            // Immediate
            case 'I': {

                bytesConsumed = handleImmediate(file, i + 1, &instructionBuilder);
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

    }

    writeFile(fileNameOut, writeBuffer, writeBufferLength);
    delete[] writeBuffer;
    delete[] file;

    return ErrorCode::NONE;

}
