#include <iostream>

#include "NES.hpp"
#include "CPU.hpp"

int main() {
    NES nes;

    nes.cpu.instructionTable[0].fnc();
    
    return 0;
}
