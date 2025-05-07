#include <cstdint>
#include <string>
#include <functional>

#ifndef TYPES_H
#define TYPES_H

namespace NESTypes {
    typedef struct Bus {
        uint8_t data;
        uint16_t address;
    } Bus;
};

namespace CPUTypes {
    typedef struct RegList {
        uint8_t A, X, Y, SP, status;
        uint16_t PC;
    } RegList;

    enum Flags {
        C = 0,
        Z = 1,
        I = 2,
        D = 3,
        B = 4,
        ONE = 5,
        V = 6,
        N = 7 
    };

    typedef struct InstructionDef {
        uint8_t size, cycles;
        std::function<void()> fnc;
    } Instruction;
};

#endif
