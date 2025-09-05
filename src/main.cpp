#include "disassembler/disassembler.h"
#include <cstdio>

int main(int argc, char** argv) {

    const char* in = "intermediate/test.irbin";
    const char* out = "out/test.asm";

    Disassembler::ErrorCode errorCode = Disassembler::generateDisassemble(in, out);
    if (static_cast<uint32_t>(errorCode)) {
        std::printf("Dissassemble failed!\n");
        std::printf("Error Code: %d\n", static_cast<uint32_t>(errorCode));
    }

    else {
        std::printf("Disassembled %s -> %s\n", in, out);
    }

    return 0;

}
