#include "assembler/logHelpers.h"

using namespace Assembler;

void Assembler::logInstruction(const Instruction& instruction, File* log) {

    log->write("Instruction: {", true);

    log->write("  opcode = ", false);
    log->write(static_cast<uint8_t>(instruction.opcode), true, WriteFormat::DECIMAL);

    log->write("  operandCount = ", false);
    log->write(instruction.operandCount, true);

    for (uint8_t i = 0; i < instruction.operandCount; i++) {
        const Operand& op = instruction.operands[i];
        log->write("  operands[", false);
        log->write(i, false);
        log->write("] = {", true);

        log->write("    type = ", false);
        switch(op.type) {

            case OperandType::REGISTER:
                log->write("Reg", true);
                log->write("    reg = ", false);
                log->write(static_cast<uint8_t>(op.reg), true, WriteFormat::DECIMAL);
                break;

            case OperandType::IMMEDIATE:
                log->write("Immediate", true);
                log->write("    immediate.size = ", false);
                log->write(op.immediate.size, true);
                log->write("    immediate.value = ", false);
                log->write(static_cast<int>(op.immediate.value));
                log->write("", true);
                break;

            case OperandType::MEMORY:
                log->write("Mem", true);
                log->write("    mem.reg = ", false);
                log->write(static_cast<uint8_t>(op.mem.reg), true, WriteFormat::DECIMAL);
                log->write("    mem.offsetSize = ", false);
                log->write(op.mem.offsetSize, true);
                log->write("    mem.offset = ", false);
                log->write(static_cast<int>(op.mem.offset));
                log->write("", true);
                break;

            default:
                log->write("Unknown", true);
                break;

        }

        log->write("  }", true);
    }

    log->write("}", true);

    return;

}

void Assembler::logInstructionSignature(const InstructionSignature& sig, File* log) {

    log->write("InstructionSignature: {", true);

    // Preference
    log->write("  preference = ", false);
    log->write(static_cast<int>(sig.preference), true);

    // Opcode
    log->write("  opcode = ", false);
    log->write(static_cast<int>(sig.opcode), true);

    // Mode
    log->write("  mode = ", false);
    log->write(static_cast<int>(sig.mode), true);

    // Operands
    for (uint8_t i = 0; i < 3; i++) {
        log->write("  operands[", false);
        log->write(i, false);
        log->write("] = ", false);
        log->write(static_cast<int>(sig.operands[i]), true);
    }

    // Opcode bytes
    log->write("  opcodeBytesLength = ", false);
    log->write(static_cast<int>(sig.opcodeBytesLength), true);

    log->write("  opcodeBytes = ", false);
    for(uint8_t i = 0; i < sig.opcodeBytesLength; i++) {
        log->write(static_cast<int>(sig.opcodeBytes[i]));
        if (i < sig.opcodeBytesLength - 1) {
            log->write(" ", false);
        }
    }
    log->writeNewLine();

    // Immediate
    log->write("  immediate = ", false);
    log->write(static_cast<int>(sig.immediate), true);

    log->write("}", true);

    return;

}
