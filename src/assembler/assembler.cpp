#include "assembler/assembler.h"

#include "assembler/lookup.h"
#include "core/File.h"

#include <cstdio>
#include <string.h>
#include <algorithm>

using namespace Assembler;


// Writes an opcode label into 'instruction'
size_t handleOpcode(uint8_t opcode, Instruction* instruction, File* log) {

    instruction->opcode = static_cast<Opcode>(opcode);

    return 1;

}

// Writes a register label into 'instruction'
size_t handleRegister(uint8_t reg, Instruction* instruction, File* log) {

    size_t insertionIndex = instruction->operandCount;

    Operand* operand = &(instruction->operands[insertionIndex]);
    operand->type = OperandType::REGISTER;
    operand->reg = static_cast<Register>(reg);

    instruction->operandCount++;

    return 1;

}

// Writes a memory reference label into 'instruction'
size_t handleMemory(uint8_t* fileData, size_t startIndex, Instruction* instruction, File* log) {

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
            
            log->write("Error: Found an invalid memory offset length character!", true);
            log->write(" >> Found character \'");
            log->write(static_cast<char>(fileData[startIndex + 1]));
            log->write("\' (int value ");
            log->write(static_cast<char>(fileData[startIndex + 1]), false, WriteFormat::DECIMAL);
            log->write("), which does not match any known length character.", true);

            break;

        }

    }

    instruction->operandCount++;

    return consumed;

}

// Writes an immediate value label into 'instruction'
size_t handleImmediate(uint8_t* fileData, size_t startIndex, Instruction* instruction, File* log) {

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
            
            log->write("Error: Found an invalid immediate length character!", true);
            log->write(" >> Found character \'");
            log->write(static_cast<char>(fileData[startIndex]));
            log->write("\' (int value ");
            log->write(static_cast<char>(fileData[startIndex]), false, WriteFormat::DECIMAL);
            log->write("), which does not match any known length character.", true);

            consumed = 1;

            break;

        }

    }

    instruction->operandCount++;

    return consumed;

}

// Assembles the instruction data from 'instruction' into 'instructionBytes'. Returns true on a failiure
bool assembleInstruction(const Instruction& instruction, InstructionBytes* instructionBytesOut, File* log) {

    memset(instructionBytesOut, 0, sizeof(InstructionBytes));
    constexpr size_t MAX_OPERAND_COUNT = (sizeof(InstructionSignature::operands) / sizeof(SignatureOperandType));
    
    const InstructionSignature* match = matchInstruction(instruction);
    if (match == nullptr) {
        log->write("Error: assembleInstruction was unable to match the given instruction to any known signature!", true);
        return true;
    }
    
    // Opcode
    {
        memcpy(
            &(instructionBytesOut->opcode), 
            &(match->opcode), 
            std::min(sizeof(match->opcode), static_cast<size_t>(match->opcodeBytesLength))
        );
        instructionBytesOut->opcodeSize = match->opcodeBytesLength;

        if (match->mode == Mode::PLUS_RD) {
            
            // Find which register is used in the instruction
            Register reg = Register::NONE;
            for (uint8_t i = 0; i < instruction.operandCount; i++) {
                if (instruction.operands[i].type == OperandType::REGISTER) {
                    reg = instruction.operands[i].reg;
                    break;
                }
            }

            if (reg == Register::NONE) {
                log->write("Error: assembleInstruction could not find a REGISTER operand type in an instruction with Mode::PLUS_RD (+rd)!", true);
                return true;
            }

            uint8_t writeIndex = std::min(sizeof(instructionBytesOut->opcode) - 1, static_cast<size_t>(instructionBytesOut->opcodeSize - 1));
            instructionBytesOut->opcode[writeIndex] += static_cast<uint8_t>(reg);

        }
    }

    // ModR/M byte
    {
        uint8_t mode = static_cast<uint8_t>(match->mode);

        // reg field
        {
            // mode in [9, 10] (PLUS_RD, NONE)
            if (mode >= static_cast<uint8_t>(Mode::PLUS_RD)) goto END_MODRM;
            
            // mode is /digit
            if (mode < static_cast<uint8_t>(Mode::SLASH_R)) {
                instructionBytesOut->modrm |= ((mode << 3) & 0b00111000);
            }

            // mode == 8 (SLASH_R)
            else {

                size_t i = 0;
                for (; i < MAX_OPERAND_COUNT; i++) {
                    if (match->operands[i] == SignatureOperandType::R32) break;
                }

                if (instruction.operands[i].type != OperandType::REGISTER) {
                    
                    log->write("Error: assembleInstruction found an invalid operand type where it expected a REGISTER type!", true);
                    log->write(" >> Found type ");
                    log->write(static_cast<int>(instruction.operands[i].type));
                    log->write(".", true);

                    return true;
                
                }

                uint8_t registerValue = static_cast<uint8_t>(instruction.operands[i].reg);
                instructionBytesOut->modrm |= ((registerValue << 3) & 0b00111000);

            }
        }

        // mode and r/m fields
        {
            size_t i = 0;
            for (; i < MAX_OPERAND_COUNT; i++) {
                if (match->operands[i] == SignatureOperandType::RM32) break;
            }

            switch (instruction.operands[i].type) {
                
                case OperandType::IMMEDIATE: {
                    log->write("Error: assembleInstruction found an IMMEDIATE operand type where it expected a REGISTER or MEMORY type!", true);
                    return true;
                }

                case OperandType::REGISTER: {

                    instructionBytesOut->modrm |= (0b11000000);

                    uint8_t registerValue = static_cast<uint8_t>(instruction.operands[i].reg);
                    instructionBytesOut->modrm |= (registerValue & 0b00000111);

                    break;

                }

                case OperandType::MEMORY: {

                    uint8_t registerValue = static_cast<uint8_t>(instruction.operands[i].mem.reg);
                    
                    // This case is valid as per x86 documentation, its just out of the current scope of this project
                    // Both of these register values indicate special cases that require extra handling
                    if (
                        instruction.operands[i].mem.reg == Register::ESP || 
                        instruction.operands[i].mem.reg == Register::EBP
                    ) {
                        
                        log->write("Error: assembleInstruction found an illegal register value for the memory field in the modr/m byte!", true);
                        log->write(" >> The instruction is using either ESP or EBP as the base register in a memory operand, which is currently not allowed.", true);

                        return true;
                    
                    }
                    
                    instructionBytesOut->modrm |= (registerValue & 0b00000111);
                    
                    uint32_t displacementSize = instruction.operands[i].mem.offsetSize;
                    switch (displacementSize) {

                        case 0:
                            instructionBytesOut->modrm |= (0b00000000);
                            break;

                        case 1:
                            instructionBytesOut->modrm |= (0b01000000);
                            break;

                        case 4:
                            instructionBytesOut->modrm |= (0b10000000);
                            break;

                        default: {

                            log->write("Error: assembleInstruction found an invalid memory offset length for the memory field in the modr/m byte!", true);
                            log->write(" >> Found length ");
                            log->write(static_cast<int>(displacementSize));
                            log->write(".", true);

                            return true;

                        }

                    }

                    break;

                }

                default: {
                    
                    log->write("Error: assembleInstruction found an unknown operand type where it expected a REGISTER or MEMORY type!", true);
                    log->write(" >> Found type ");
                    log->write(static_cast<int>(instruction.operands[i].type));
                    log->write(".", true);

                    return true;

                }

            }

        }

    }
    END_MODRM:

    // Displacement
    {
        bool eligibleForDisplacement = false;
        size_t onOperand;
        size_t range = (sizeof(match->operands) / sizeof(SignatureOperandType));
        for (size_t i = 0; i < range; i++) {

            const Operand* currentOperand = &(instruction.operands[i]);
            if (currentOperand->type != OperandType::MEMORY) continue;

            switch (match->operands[i]) {

                case SignatureOperandType::RM32:
                case SignatureOperandType::MEM:
                    eligibleForDisplacement = true;
                    onOperand = i;
                    break;

                default:
                    break;

            }
        }

        if (eligibleForDisplacement) {
            Operand focusOperand = instruction.operands[onOperand];
            instructionBytesOut->displacement = focusOperand.mem.offset;
            instructionBytesOut->displacementSize = focusOperand.mem.offsetSize;
        }
    }

    // Immediate
    {
        bool eligibleForImmediate = false;
        size_t onOperand;
        size_t range = (sizeof(match->operands) / sizeof(SignatureOperandType));
        for (size_t i = 0; i < range; i++) {

            const Operand* currentOperand = &(instruction.operands[i]);
            if (currentOperand->type != OperandType::IMMEDIATE) continue;

            switch (match->operands[i]) {

                case SignatureOperandType::IMM8:
                    if (currentOperand->immediate.size > 1) {
                        
                        log->write("Error: assembleInstruction found an IMMEDIATE operand larger than the allowed size!", true);
                        log->write(" >> The IMMEDIATE was size ", true);
                        log->write(static_cast<int>(currentOperand->immediate.size));
                        log->write(", but the signature expected size 1", true);

                        return true;

                    }
                    [[fallthrough]];

                case SignatureOperandType::IMM16:
                    if (currentOperand->immediate.size > 2) {
                        
                        log->write("Error: assembleInstruction found an IMMEDIATE operand larger than the allowed size!", true);
                        log->write(" >> The IMMEDIATE was size ", true);
                        log->write(static_cast<int>(currentOperand->immediate.size));
                        log->write(", but the signature expected size 2", true);

                        return true;

                    }
                    [[fallthrough]];

                case SignatureOperandType::IMM32: {
                    eligibleForImmediate = true;
                    onOperand = i;
                    break;
                }
                
                default: {

                    log->write("Error: assembleInstruction found an out of place IMMEDIATE operand type!", true);
                    log->write(" >> The IMMEDIATE type appeared where the SignatureOperandType was ", true);
                    log->write(static_cast<int>(match->operands[i]));
                    log->write(".", true);

                    return true;

                }

            }

        }

        if (eligibleForImmediate) {
            instructionBytesOut->immediate = instruction.operands[onOperand].immediate.value;
            instructionBytesOut->immediateSize = instruction.operands[onOperand].immediate.size;
        }
    }

    return false;

}

// Writes the IntructionBytes into the writeBuffer. Conditionally writes parts depending on the contents
void writeInstruction(const InstructionBytes& instructionBytes, uint8_t* writeBuffer, size_t* writeBufferLength, File* log) {

    return;

}

ErrorCode Assembler::generateAssemble(const char* fileNameIn, const char* fileNameOut) {

    File log("logs/assembler.log");
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

    size_t bytesConsumed;
    Instruction instructionBuilder {};
    InstructionBytes instructionBytes {};

    File fileOut(fileNameOut);
    fileOut.clear();

    for (size_t i = 0; i < fileLength; i++) {

        switch (file[i]) {
            
            // Opcode
            case 'O': {

                assembleInstruction(instructionBuilder, &instructionBytes, &log);
                writeInstruction(instructionBytes, writeBuffer, &writeBufferLength, &log);

                bytesConsumed = handleOpcode(file[i + 1], &instructionBuilder, &log);
                break;

            }

            // Register
            case 'R': {
                bytesConsumed = handleRegister(file[i + 1], &instructionBuilder, &log);
                break;
            }

            // Memory
            case 'M': {
                bytesConsumed = handleMemory(file, i + 1, &instructionBuilder, &log);
                break;
            }

            // Immediate
            case 'I': {
                bytesConsumed = handleImmediate(file, i + 1, &instructionBuilder, &log);
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
            fileOut.write(writeBuffer, writeBufferLength);
            memset(writeBuffer, 0x00, WRITE_BUFFER_SIZE);
            writeBufferLength = 0;
        }

    }

    assembleInstruction(instructionBuilder, &instructionBytes, &log);
    writeInstruction(instructionBytes, writeBuffer, &writeBufferLength, &log);

    fileOut.write(writeBuffer, writeBufferLength);
    delete[] writeBuffer;
    delete[] file;

    return ErrorCode::NONE;

}
