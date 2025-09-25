#pragma once

#include "assembler/data.h"

#include <cstdint>
#include <cstddef>



namespace Assembler {

enum class ErrorCode : uint32_t {
    NONE = 0,
    INVALID_FILE = 1,
    FOUND_INVALID_PREFIX = 2,
};

// Takes the data from fileNameIn and puts the assembly into fileNameOut
ErrorCode generateAssemble(const char* fileNameIn, const char* fileNameOut);

}
