#include "assembler/lookup.h"

using namespace Assembler;


bool Assembler::operandIs(const Operand& operand, SignatureOperandType signatureOperandType) {

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
            // log error
            return false;
        }

    }

}
