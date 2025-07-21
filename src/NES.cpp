#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

#include "NES.hpp"
#include "CPU.hpp"
#include "ROM.hpp"


NES::NES() : cpu(this->bus), rom(this->bus) {
    bus = {0, 0, 1};
    ram.fill(0);
}


void NES::init() {
    handleCpuPcStart();
}


void NES::mainLoop() {
    while(cpu.registers.PC != 0xbfe1) {
        handleCpuGetNextInstruction();

        uint16_t address = bus.address;
        // memory is mirrored 4 times between address 0 - 1fff in chunks of 0x800 bytes
        if(bus.rwSignal == 1 && address >= 0 && address <= 0x1fff) {
            bus.data = ram[address % 0x800];
        }

        cpu.executeInstruction();

        if(bus.rwSignal == 0 && address >= 0 && address <= 0x1fff) {
            ram[address % 0x800] = bus.data;
        }
        
        if(cpu.instruction[0] != 0xea) {
            cpu.printRegisters();
        }
    }

    dumpRAM();
}


void NES::handleCpuGetNextInstruction() {
    cpu.aBusLoadPC();
    rom.dBusLoadByteAtAddress();

    // get opcode
    uint8_t opcode = bus.data,
            size = cpu.instructionTable[opcode].size,
            cycles = cpu.instructionTable[opcode].cycles;
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
        cpu.aBusLoadInstructionArgument();
    } else if(size == 2 && cpu.instructionTable[opcode].mode != CPUTypes::AddressingMode::Immediate) {
        // non immediate instructions
        // data from immediate instruciton
        // is loaded from rom.dBusLoadByteAtAddress
        cpu.aBusLoadInstructionArgument();
    }

    cpu.rwBusSetSignal();

    handleCpuIndexedAddressing();
}


void NES::handleCpuIndexedAddressing() {
    uint8_t opcode = cpu.instruction[0];
    CPUTypes::AddressingMode mode = cpu.instructionTable[opcode].mode;

    switch(mode) {
        case CPUTypes::AddressingMode::ZeroPageIndexedX:
        case CPUTypes::AddressingMode::AbsoluteIndexedX:
            cpu.aBusAddXReg(mode);
            break;
        case CPUTypes::AddressingMode::ZeroPageIndexedY:
        case CPUTypes::AddressingMode::AbsoluteIndexedY:
            cpu.aBusAddYReg(mode);
            break;
        case CPUTypes::AddressingMode::IndexedIndirectX: {
                cpu.aBusAddXReg(CPUTypes::AddressingMode::ZeroPageIndexedX);
                uint16_t lowAddress = bus.address;

                cpu.aBusLoadInstructionArgument();
                bus.address += 1;
                cpu.aBusAddXReg(CPUTypes::AddressingMode::ZeroPageIndexedX);
                uint16_t highAddress = bus.address;

                bus.address = (ram[highAddress] << 8) | ram[lowAddress];
                break;
            }
        case CPUTypes::AddressingMode::IndirectIndexedY: {
                uint16_t address = bus.address;
                bus.address = (ram[address + 1 % 256] << 8) | ram[address];
                cpu.aBusAddYReg(CPUTypes::AddressingMode::AbsoluteIndexedY);
                break;
            }
        default:
            break;
    }
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


void NES::dumpRAM() {
    for(int i = 0, line = 0; i < 0x801; i++) {
        if(ram[i] != 0) {
            printf("%04x | %x\n", i, ram[i]);
        }
    }
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
