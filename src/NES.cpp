#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include "NES.hpp"
#include "CPU.hpp"
#include "ROM.hpp"


NES::NES() : cpu(this->bus), rom(this->bus) {
    bus = {0, 0, 1};
}


void NES::init() {
    handleCpuPcStart();

    ram[0x5ff] = 0xe9;
}


void NES::mainLoop() {
    handleCpuGetNextInstruction();

    uint16_t address = bus.address;
    if(bus.rwSignal == 1 && address >= 0 && address <= 0x7ff) {
        bus.data = ram[address];
    }
    
    cpu.executeInstruction();
    cpu.printRegisters();

    if(bus.rwSignal == 0 && address >= 0 && address <= 0x7ff) {
        ram[address] = bus.data;
    }
}


void NES::handleCpuGetNextInstruction() {
    cpu.aBusLoadPC();
    rom.dBusLoadByteAtAddress();

    // get opcode
    uint8_t size = cpu.instructionTable[bus.data].size;
    cpu.dBusStoreInstruction(0);
    cpu.incrementPC();

    // get instruction arguments
    for(int i = 1; i < size; i++) {
        cpu.aBusLoadPC();
        rom.dBusLoadByteAtAddress();
        cpu.dBusStoreInstruction(i);

        cpu.incrementPC();
    }

    if(size == 3) {
        cpu.flattenInstructionArgument();
    }
    if(size >= 2) {
        cpu.aBusLoadInstructionArgument();
    }

    uint8_t opcode = cpu.instruction[0];
    bus.rwSignal = cpu.instructionTable[opcode].rw;
}


void NES::handleCpuPcStart() {
    for(int i = 0; i < 2; i++) {
        cpu.aBusLoadPC();
        rom.dBusLoadByteAtAddress();

        cpu.aBusStoreResetVector();

        cpu.incrementPC();
    }

    cpu.registers.PC = (cpu.resetVector[0] << 8) | (cpu.resetVector[1]);
}


void NES::hexStringFileToHexFile(std::string fileName) {
    std::ifstream ifile(fileName.c_str());

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
    
    for(int i = flen; i < 0xbfe0; i++) {
        bytes.push_back(0xea);
    }

    bytes[0xfffc - 0x4020] = 0x20;
    bytes[0xfffd - 0x4020] = 0x40;

    fileName.replace(fileName.size() - 1, 1, "nes");
    std::ofstream ofile(fileName, std::ofstream::binary);

    if(ofile.is_open()) {
        ofile.write((char*)bytes.data(), 0xbfe0);

        ofile.close();
    }
}
