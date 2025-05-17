#include <iostream>

#include "ROM.hpp"

ROM::ROM(NESTypes::Bus &b) : bus(b) {   
    std::cout << "rom" << std::endl;
}


