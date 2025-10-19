#include "assembler/lookup.h"

using namespace Assembler;


bool Assembler::operandIs(const Operand& operand, SignatureOperandType signatureOperandType, File* log) {

    switch (operand.type) {

        case OperandType::REGISTER: {

            switch (signatureOperandType) {

                case SignatureOperandType::EAX:
                    return operand.reg == Register::EAX;

                case SignatureOperandType::EBX:
                    return operand.reg == Register::EBX;

                case SignatureOperandType::ECX:
                    return operand.reg == Register::ECX;

                case SignatureOperandType::EDX:
                    return operand.reg == Register::EDX;
                
                case SignatureOperandType::ESI:
                    return operand.reg == Register::ESI;

                case SignatureOperandType::EDI:
                    return operand.reg == Register::EDI;

                case SignatureOperandType::ESP:
                    return operand.reg == Register::ESP;

                case SignatureOperandType::EBP:
                    return operand.reg == Register::EBP;

                case SignatureOperandType::R32:
                case SignatureOperandType::RM32:
                    return true;

                default:
                    return false;

            }

        }

        case OperandType::IMMEDIATE: {
            
            switch (operand.immediate.size) {

                case 1: {
                    return (
                        signatureOperandType == SignatureOperandType::IMM8  ||
                        signatureOperandType == SignatureOperandType::IMM16 ||
                        signatureOperandType == SignatureOperandType::IMM32 ||
                        signatureOperandType == SignatureOperandType::REL8  ||
                        signatureOperandType == SignatureOperandType::REL32
                    );
                }

                case 4: {
                    
                    if (operand.immediate.value == (operand.immediate.value & 0xFFFF)) {
                        if (signatureOperandType == SignatureOperandType::IMM16) return true;
                    }

                    return (
                        signatureOperandType == SignatureOperandType::IMM32 ||
                        signatureOperandType == SignatureOperandType::REL32
                    );
                
                }

                default: {
                    // log error
                    return false;
                }

            }

        }

        case OperandType::MEMORY: {
            
            return (
                signatureOperandType == SignatureOperandType::RM32 ||
                signatureOperandType == SignatureOperandType::MEM
            );

        }

        default: {
            
            if (log != nullptr) {
                log->write("Assembler::operandIs() failed to match 'operand.type' to any known type!", true);
                log->write(" >> Found type ");
                log->write(static_cast<int>(operand.type));
                log->write(".", true);
            }

            return false;

        }

    }

}

const InstructionSignature* Assembler::matchInstruction(const Instruction& instruction, File* log) {

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

            if (!operandIs(instruction.operands[j], lookup[i].operands[j], log)) {
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
