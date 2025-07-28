#include <cstdint>
#include <string>
#include <functional>

#ifndef TYPES_H
#define TYPES_H

namespace NESTypes {
    typedef struct Bus {
        uint8_t data;
        uint16_t address;
        // 0 = write, 1 = read, 1+ = has to read/write more than 1 byte to ram
        uint8_t rwSignal;
    } Bus;
};

namespace CPUTypes {
    typedef struct Registers {
        uint8_t A, X, Y, SP, status;
        uint16_t PC;
    } Registers;

    enum RegisterName {
        A, X, Y, SP, Status, PC
    };

    enum Flag {
        C = 0,
        Z = 1,
        I = 2,
        D = 3,
        B = 4,
        One = 5,
        V = 6,
        N = 7 
    };

    enum AddressingMode {
        Absolute,
        Accumulator,
        Immediate,
        Implied,
        Indirect,
        Relative,
        ZeroPage,
        AbsoluteIndexedX,
        AbsoluteIndexedY,
        IndexedIndirectX,
        IndexedIndirectY,
        IndirectIndexedX,
        IndirectIndexedY,
        ZeroPageIndexedX,
        ZeroPageIndexedY
    };

    typedef struct InstructionDef {
        uint8_t size, cycles, rw;
        AddressingMode mode;
        std::function<void()> fnc;
    } Instruction;
};

#endif
