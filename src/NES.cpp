#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include "NES.hpp"
#include "CPU.hpp"
#include "ROM.hpp"


NES::NES() : cpu(this->bus), rom(this->bus) {
    bus = {0, 0};
}


void NES::hexStringFileToHexFile(std::string filename) {
    std::ifstream ifile(filename.c_str());

    ifile.seekg(0, ifile.end);
    // hex string is char char space, so every 3 characters is 1 byte
    int flen = ifile.tellg() / 3;
    ifile.seekg(0, ifile.beg);

    std::vector<uint8_t> bytes(flen);

    if(ifile.is_open()) {
        std::string s;
        char buf[2];
 
        int ctr = 0;
        while(ifile.get(buf, 4)) {
            bytes[ctr] = std::stoi(buf, nullptr, 16);
            ctr++;
        }

        ifile.close();
    }

    filename.replace(filename.size() - 1, 1, "nes");
    std::ofstream ofile(filename, std::ofstream::binary);

    if(ofile.is_open()) {
        ofile.write((char*)bytes.data(), flen);
        
        ofile.close();
    }
}
