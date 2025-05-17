#include <iostream>

#include "NES.hpp"
#include "CPU.hpp"
#include "ROM.hpp"

int main(int argc, char** argv) {
    NES nes;

    if(argc == 2) {
        NES::hexStringFileToHexFile(argv[1]);
    }

    return 0;
}
