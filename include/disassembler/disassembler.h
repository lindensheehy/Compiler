#pragma once

#include <cstdint>


namespace Disassembler {

enum class ErrorCode : uint32_t {
    NONE = 0,
    INVALID_FILE = 1,
    FOUND_INVALID_PREFIX = 2,
};

// Takes the data from fileNameIn and puts the disassembly into fileNameOut
ErrorCode generateDisassemble(const char* fileNameIn, const char* fileNameOut);

}
