#include <array>
#include <cstdint>
#include <string>

#include "types.hpp"
#include "CPU.hpp"
#include "ROM.hpp"

#ifndef NES_H
#define NES_H


class NES {
    public:
        CPU cpu;
        ROM rom;
        NESTypes::Bus bus;
        std::array<uint8_t, 0x800> ram;

        NES();

        void init();
        void mainLoop();

        void handleCpuGetNextInstruction();
        void handleCpuPcStart();

        // standin for an eventual assembler
        static void hexStringFileToHexFile(std::string filename);
};

#endif 
