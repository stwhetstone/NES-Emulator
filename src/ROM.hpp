#include <string>
#include <vector>

#include "types.hpp"

#ifndef ROM_H
#define ROM_H

class ROM {
    public:
        NESTypes::Bus &bus;
        std::vector<int> data;

        ROM(NESTypes::Bus &b);
};

#endif
