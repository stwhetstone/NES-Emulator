#include <iostream>

#include "NES.hpp"


NES::NES() : cpu(bus) {
    bus = {0, 0};

    std::cout << "nes" << std::endl;
}
