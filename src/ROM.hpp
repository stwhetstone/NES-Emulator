#include <string>
#include <array>

#include "types.hpp"

#ifndef ROM_H
#define ROM_H

class ROM {
    public:
        NESTypes::Bus &bus;
        std::array<uint8_t, 0xbfe0> data;

        ROM(NESTypes::Bus &b);

        void busLoadByte();
        void loadFileData(std::string fileName);
};

#endif
