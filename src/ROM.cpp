#include <iostream>
#include <fstream>
#include <string>

#include "ROM.hpp"

ROM::ROM(NESTypes::Bus &b) : bus(b) {   
    ;
}

void ROM::getFileData(std::string fileName) {
    std::ifstream inf(fileName.c_str(), std::ifstream::binary);

    char buf;
    int ctr = 0;
    if(inf.is_open()) {
        while(inf.read(&buf, 1)) {
            data[ctr] = buf;
            ctr++;
        }
    }
}


void ROM::dLoadByteAtAddress() {
    // rom memory space starts at 0x4020
    uint16_t pc = bus.address - 0x4020;

    bus.data = this->data[pc];
}


