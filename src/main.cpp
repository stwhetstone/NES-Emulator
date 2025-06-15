#include <iostream>

#include "NES.hpp"
#include "CPU.hpp"
#include "ROM.hpp"

int main(int argc, char** argv) {
    NES nes;


    if(argc == 2) {
        nes.rom.loadFileData(argv[1]);
    } else if(argc == 3) {
        if(strcmp("-b", argv[1]) == 0) {
            NES::hexStringFileToHexFile(argv[2]);
        }
        return 0;
    }

    nes.init();
    nes.mainLoop();

    return 0;
}
