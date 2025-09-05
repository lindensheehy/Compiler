#include "disassembler/disassembler.h"
#include <cstdio>

int main(int argc, char** argv) {
    const char* in  = (argc > 1) ? argv[1] : "test.irbin";
    const char* out = (argc > 2) ? argv[2] : "test.asm";
    Disassembler::generateDisassemble(in, out);
    std::printf("Disassembled %s -> %s\n", in, out);
    return 0;
}
