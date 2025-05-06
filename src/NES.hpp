#include <array>
#include <cstdint>

#include "CPU.hpp"
#include "types.hpp"

#ifndef NES_H
#define NES_H


class NES {
    public:
        CPU cpu;
        NESTypes::Bus bus;
        std::array<uint8_t, 0x800> ram;

        NES();
};

#endif 
