#pragma once

#include <cstdint>


struct Instruction {

    public:
        uint8_t opcode[4];
        uint8_t modrm;
    private:
        uint8_t padding[3];
    public:
        uint32_t displacement;
        uint32_t immediate;

};
