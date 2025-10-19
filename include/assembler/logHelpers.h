#pragma once

#include "assembler/data.h"
#include "assembler/lookup.h"
#include "core/File.h"

namespace Assembler {

// Logs the contents of 'instruction' in a readable format
void logInstruction(const Instruction& instruction, File* log);

// Logs the contents of 'sig' in a readable format
void logInstructionSignature(const InstructionSignature& sig, File* log);

}
