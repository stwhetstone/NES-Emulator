#include <iostream>
#include <array>
#include <string>

#include "CPU.hpp"


CPU::CPU(NESTypes::Bus &b) : bus(b) {
    registers =  {};

    registers.A = 0;
    registers.X = 0;
    registers.Y = 0;

    registers.SP = 0xff;

    // NV1B DIZC
    registers.status = 0b00100100;

    registers.PC = 0xfffc;
 
    stepCycles = 0;

    initInstructionTable();
}

void CPU::initInstructionTable() {
    instructionTable.reserve(0xff);
    instructionTable[0x0] = (CPUTypes::InstructionDef){};
    instructionTable[0x1] = (CPUTypes::InstructionDef){};
    instructionTable[0x3] = (CPUTypes::InstructionDef){};
    instructionTable[0x4] = (CPUTypes::InstructionDef){};
    instructionTable[0x5] = (CPUTypes::InstructionDef){};
    instructionTable[0x6] = (CPUTypes::InstructionDef){};
    instructionTable[0x7] = (CPUTypes::InstructionDef){};
    instructionTable[0x8] = (CPUTypes::InstructionDef){};
    instructionTable[0x9] = (CPUTypes::InstructionDef){};
    instructionTable[0xa] = (CPUTypes::InstructionDef){};
    instructionTable[0xb] = (CPUTypes::InstructionDef){};
    instructionTable[0xc] = (CPUTypes::InstructionDef){};
    instructionTable[0xd] = (CPUTypes::InstructionDef){};
    instructionTable[0xe] = (CPUTypes::InstructionDef){};
    instructionTable[0xf] = (CPUTypes::InstructionDef){};
    instructionTable[0x10] = (CPUTypes::InstructionDef){};
    instructionTable[0x11] = (CPUTypes::InstructionDef){};
    instructionTable[0x13] = (CPUTypes::InstructionDef){};
    instructionTable[0x14] = (CPUTypes::InstructionDef){};
    instructionTable[0x15] = (CPUTypes::InstructionDef){};
    instructionTable[0x16] = (CPUTypes::InstructionDef){};
    instructionTable[0x17] = (CPUTypes::InstructionDef){};
    instructionTable[0x18] = (CPUTypes::InstructionDef){};
    instructionTable[0x19] = (CPUTypes::InstructionDef){};
    instructionTable[0x1a] = (CPUTypes::InstructionDef){};
    instructionTable[0x1b] = (CPUTypes::InstructionDef){};
    instructionTable[0x1c] = (CPUTypes::InstructionDef){};
    instructionTable[0x1d] = (CPUTypes::InstructionDef){};
    instructionTable[0x1e] = (CPUTypes::InstructionDef){};
    instructionTable[0x1f] = (CPUTypes::InstructionDef){};
    instructionTable[0x20] = (CPUTypes::InstructionDef){};
    instructionTable[0x21] = (CPUTypes::InstructionDef){};
    instructionTable[0x23] = (CPUTypes::InstructionDef){};
    instructionTable[0x24] = (CPUTypes::InstructionDef){};
    instructionTable[0x25] = (CPUTypes::InstructionDef){};
    instructionTable[0x26] = (CPUTypes::InstructionDef){};
    instructionTable[0x27] = (CPUTypes::InstructionDef){};
    instructionTable[0x28] = (CPUTypes::InstructionDef){};
    instructionTable[0x29] = (CPUTypes::InstructionDef){};
    instructionTable[0x2a] = (CPUTypes::InstructionDef){};
    instructionTable[0x2b] = (CPUTypes::InstructionDef){};
    instructionTable[0x2c] = (CPUTypes::InstructionDef){};
    instructionTable[0x2d] = (CPUTypes::InstructionDef){};
    instructionTable[0x2e] = (CPUTypes::InstructionDef){};
    instructionTable[0x2f] = (CPUTypes::InstructionDef){};
    instructionTable[0x30] = (CPUTypes::InstructionDef){};
    instructionTable[0x31] = (CPUTypes::InstructionDef){};
    instructionTable[0x33] = (CPUTypes::InstructionDef){};
    instructionTable[0x34] = (CPUTypes::InstructionDef){};
    instructionTable[0x35] = (CPUTypes::InstructionDef){};
    instructionTable[0x36] = (CPUTypes::InstructionDef){};
    instructionTable[0x37] = (CPUTypes::InstructionDef){};
    instructionTable[0x38] = (CPUTypes::InstructionDef){};
    instructionTable[0x39] = (CPUTypes::InstructionDef){};
    instructionTable[0x3a] = (CPUTypes::InstructionDef){};
    instructionTable[0x3b] = (CPUTypes::InstructionDef){};
    instructionTable[0x3c] = (CPUTypes::InstructionDef){};
    instructionTable[0x3d] = (CPUTypes::InstructionDef){};
    instructionTable[0x3e] = (CPUTypes::InstructionDef){};
    instructionTable[0x3f] = (CPUTypes::InstructionDef){};
    instructionTable[0x40] = (CPUTypes::InstructionDef){};
    instructionTable[0x41] = (CPUTypes::InstructionDef){};
    instructionTable[0x43] = (CPUTypes::InstructionDef){};
    instructionTable[0x44] = (CPUTypes::InstructionDef){};
    instructionTable[0x45] = (CPUTypes::InstructionDef){};
    instructionTable[0x46] = (CPUTypes::InstructionDef){};
    instructionTable[0x47] = (CPUTypes::InstructionDef){};
    instructionTable[0x48] = (CPUTypes::InstructionDef){};
    instructionTable[0x49] = (CPUTypes::InstructionDef){};
    instructionTable[0x4a] = (CPUTypes::InstructionDef){};
    instructionTable[0x4b] = (CPUTypes::InstructionDef){};
    instructionTable[0x4c] = (CPUTypes::InstructionDef){};
    instructionTable[0x4d] = (CPUTypes::InstructionDef){};
    instructionTable[0x4e] = (CPUTypes::InstructionDef){};
    instructionTable[0x4f] = (CPUTypes::InstructionDef){};
    instructionTable[0x50] = (CPUTypes::InstructionDef){};
    instructionTable[0x51] = (CPUTypes::InstructionDef){};
    instructionTable[0x53] = (CPUTypes::InstructionDef){};
    instructionTable[0x54] = (CPUTypes::InstructionDef){};
    instructionTable[0x55] = (CPUTypes::InstructionDef){};
    instructionTable[0x56] = (CPUTypes::InstructionDef){};
    instructionTable[0x57] = (CPUTypes::InstructionDef){};
    instructionTable[0x58] = (CPUTypes::InstructionDef){};
    instructionTable[0x59] = (CPUTypes::InstructionDef){};
    instructionTable[0x5a] = (CPUTypes::InstructionDef){};
    instructionTable[0x5b] = (CPUTypes::InstructionDef){};
    instructionTable[0x5c] = (CPUTypes::InstructionDef){};
    instructionTable[0x5d] = (CPUTypes::InstructionDef){};
    instructionTable[0x5e] = (CPUTypes::InstructionDef){};
    instructionTable[0x5f] = (CPUTypes::InstructionDef){};
    instructionTable[0x60] = (CPUTypes::InstructionDef){};
    instructionTable[0x61] = (CPUTypes::InstructionDef){};
    instructionTable[0x63] = (CPUTypes::InstructionDef){};
    instructionTable[0x64] = (CPUTypes::InstructionDef){};
    instructionTable[0x65] = (CPUTypes::InstructionDef){};
    instructionTable[0x66] = (CPUTypes::InstructionDef){};
    instructionTable[0x67] = (CPUTypes::InstructionDef){};
    instructionTable[0x68] = (CPUTypes::InstructionDef){};
    instructionTable[0x69] = (CPUTypes::InstructionDef){};
    instructionTable[0x6a] = (CPUTypes::InstructionDef){};
    instructionTable[0x6b] = (CPUTypes::InstructionDef){};
    instructionTable[0x6c] = (CPUTypes::InstructionDef){};
    instructionTable[0x6d] = (CPUTypes::InstructionDef){};
    instructionTable[0x6e] = (CPUTypes::InstructionDef){};
    instructionTable[0x6f] = (CPUTypes::InstructionDef){};
    instructionTable[0x70] = (CPUTypes::InstructionDef){};
    instructionTable[0x71] = (CPUTypes::InstructionDef){};
    instructionTable[0x73] = (CPUTypes::InstructionDef){};
    instructionTable[0x74] = (CPUTypes::InstructionDef){};
    instructionTable[0x75] = (CPUTypes::InstructionDef){};
    instructionTable[0x76] = (CPUTypes::InstructionDef){};
    instructionTable[0x77] = (CPUTypes::InstructionDef){};
    instructionTable[0x78] = (CPUTypes::InstructionDef){};
    instructionTable[0x79] = (CPUTypes::InstructionDef){};
    instructionTable[0x7a] = (CPUTypes::InstructionDef){};
    instructionTable[0x7b] = (CPUTypes::InstructionDef){};
    instructionTable[0x7c] = (CPUTypes::InstructionDef){};
    instructionTable[0x7d] = (CPUTypes::InstructionDef){};
    instructionTable[0x7e] = (CPUTypes::InstructionDef){};
    instructionTable[0x7f] = (CPUTypes::InstructionDef){};
    instructionTable[0x80] = (CPUTypes::InstructionDef){};
    instructionTable[0x81] = (CPUTypes::InstructionDef){};
    instructionTable[0x82] = (CPUTypes::InstructionDef){};
    instructionTable[0x83] = (CPUTypes::InstructionDef){};
    instructionTable[0x84] = (CPUTypes::InstructionDef){};
    instructionTable[0x85] = (CPUTypes::InstructionDef){};
    instructionTable[0x86] = (CPUTypes::InstructionDef){};
    instructionTable[0x87] = (CPUTypes::InstructionDef){};
    instructionTable[0x88] = (CPUTypes::InstructionDef){};
    instructionTable[0x89] = (CPUTypes::InstructionDef){};
    instructionTable[0x8a] = (CPUTypes::InstructionDef){};
    instructionTable[0x8b] = (CPUTypes::InstructionDef){};
    instructionTable[0x8c] = (CPUTypes::InstructionDef){};
    instructionTable[0x8d] = (CPUTypes::InstructionDef){};
    instructionTable[0x8e] = (CPUTypes::InstructionDef){};
    instructionTable[0x8f] = (CPUTypes::InstructionDef){};
    instructionTable[0x90] = (CPUTypes::InstructionDef){};
    instructionTable[0x91] = (CPUTypes::InstructionDef){};
    instructionTable[0x93] = (CPUTypes::InstructionDef){};
    instructionTable[0x94] = (CPUTypes::InstructionDef){};
    instructionTable[0x95] = (CPUTypes::InstructionDef){};
    instructionTable[0x96] = (CPUTypes::InstructionDef){};
    instructionTable[0x97] = (CPUTypes::InstructionDef){};
    instructionTable[0x98] = (CPUTypes::InstructionDef){};
    instructionTable[0x99] = (CPUTypes::InstructionDef){};
    instructionTable[0x9a] = (CPUTypes::InstructionDef){};
    instructionTable[0x9b] = (CPUTypes::InstructionDef){};
    instructionTable[0x9c] = (CPUTypes::InstructionDef){};
    instructionTable[0x9d] = (CPUTypes::InstructionDef){};
    instructionTable[0x9e] = (CPUTypes::InstructionDef){};
    instructionTable[0x9f] = (CPUTypes::InstructionDef){};
    instructionTable[0xa0] = (CPUTypes::InstructionDef){};
    instructionTable[0xa1] = (CPUTypes::InstructionDef){};
    instructionTable[0xa2] = (CPUTypes::InstructionDef){};
    instructionTable[0xa3] = (CPUTypes::InstructionDef){};
    instructionTable[0xa4] = (CPUTypes::InstructionDef){};
    instructionTable[0xa5] = (CPUTypes::InstructionDef){};
    instructionTable[0xa6] = (CPUTypes::InstructionDef){};
    instructionTable[0xa7] = (CPUTypes::InstructionDef){};
    instructionTable[0xa8] = (CPUTypes::InstructionDef){};
    instructionTable[0xa9] = (CPUTypes::InstructionDef){};
    instructionTable[0xaa] = (CPUTypes::InstructionDef){};
    instructionTable[0xab] = (CPUTypes::InstructionDef){};
    instructionTable[0xac] = (CPUTypes::InstructionDef){};
    instructionTable[0xad] = (CPUTypes::InstructionDef){};
    instructionTable[0xae] = (CPUTypes::InstructionDef){};
    instructionTable[0xaf] = (CPUTypes::InstructionDef){};
    instructionTable[0xb0] = (CPUTypes::InstructionDef){};
    instructionTable[0xb1] = (CPUTypes::InstructionDef){};
    instructionTable[0xb3] = (CPUTypes::InstructionDef){};
    instructionTable[0xb4] = (CPUTypes::InstructionDef){};
    instructionTable[0xb5] = (CPUTypes::InstructionDef){};
    instructionTable[0xb6] = (CPUTypes::InstructionDef){};
    instructionTable[0xb7] = (CPUTypes::InstructionDef){};
    instructionTable[0xb8] = (CPUTypes::InstructionDef){};
    instructionTable[0xb9] = (CPUTypes::InstructionDef){};
    instructionTable[0xba] = (CPUTypes::InstructionDef){};
    instructionTable[0xbb] = (CPUTypes::InstructionDef){};
    instructionTable[0xbc] = (CPUTypes::InstructionDef){};
    instructionTable[0xbd] = (CPUTypes::InstructionDef){};
    instructionTable[0xbe] = (CPUTypes::InstructionDef){};
    instructionTable[0xbf] = (CPUTypes::InstructionDef){};
    instructionTable[0xc0] = (CPUTypes::InstructionDef){};
    instructionTable[0xc1] = (CPUTypes::InstructionDef){};
    instructionTable[0xc2] = (CPUTypes::InstructionDef){};
    instructionTable[0xc3] = (CPUTypes::InstructionDef){};
    instructionTable[0xc4] = (CPUTypes::InstructionDef){};
    instructionTable[0xc5] = (CPUTypes::InstructionDef){};
    instructionTable[0xc6] = (CPUTypes::InstructionDef){};
    instructionTable[0xc7] = (CPUTypes::InstructionDef){};
    instructionTable[0xc8] = (CPUTypes::InstructionDef){};
    instructionTable[0xc9] = (CPUTypes::InstructionDef){};
    instructionTable[0xca] = (CPUTypes::InstructionDef){};
    instructionTable[0xcb] = (CPUTypes::InstructionDef){};
    instructionTable[0xcc] = (CPUTypes::InstructionDef){};
    instructionTable[0xcd] = (CPUTypes::InstructionDef){};
    instructionTable[0xce] = (CPUTypes::InstructionDef){};
    instructionTable[0xcf] = (CPUTypes::InstructionDef){};
    instructionTable[0xd0] = (CPUTypes::InstructionDef){};
    instructionTable[0xd1] = (CPUTypes::InstructionDef){};
    instructionTable[0xd3] = (CPUTypes::InstructionDef){};
    instructionTable[0xd4] = (CPUTypes::InstructionDef){};
    instructionTable[0xd5] = (CPUTypes::InstructionDef){};
    instructionTable[0xd6] = (CPUTypes::InstructionDef){};
    instructionTable[0xd7] = (CPUTypes::InstructionDef){};
    instructionTable[0xd8] = (CPUTypes::InstructionDef){};
    instructionTable[0xd9] = (CPUTypes::InstructionDef){};
    instructionTable[0xda] = (CPUTypes::InstructionDef){};
    instructionTable[0xdb] = (CPUTypes::InstructionDef){};
    instructionTable[0xdc] = (CPUTypes::InstructionDef){};
    instructionTable[0xdd] = (CPUTypes::InstructionDef){};
    instructionTable[0xde] = (CPUTypes::InstructionDef){};
    instructionTable[0xdf] = (CPUTypes::InstructionDef){};
    instructionTable[0xe0] = (CPUTypes::InstructionDef){};
    instructionTable[0xe1] = (CPUTypes::InstructionDef){};
    instructionTable[0xe2] = (CPUTypes::InstructionDef){};
    instructionTable[0xe3] = (CPUTypes::InstructionDef){};
    instructionTable[0xe4] = (CPUTypes::InstructionDef){};
    instructionTable[0xe5] = (CPUTypes::InstructionDef){};
    instructionTable[0xe6] = (CPUTypes::InstructionDef){};
    instructionTable[0xe7] = (CPUTypes::InstructionDef){};
    instructionTable[0xe8] = (CPUTypes::InstructionDef){};
    instructionTable[0xe9] = (CPUTypes::InstructionDef){};
    instructionTable[0xea] = (CPUTypes::InstructionDef){};
    instructionTable[0xeb] = (CPUTypes::InstructionDef){};
    instructionTable[0xec] = (CPUTypes::InstructionDef){};
    instructionTable[0xed] = (CPUTypes::InstructionDef){};
    instructionTable[0xee] = (CPUTypes::InstructionDef){};
    instructionTable[0xef] = (CPUTypes::InstructionDef){};
    instructionTable[0xf0] = (CPUTypes::InstructionDef){};
    instructionTable[0xf1] = (CPUTypes::InstructionDef){};
    instructionTable[0xf3] = (CPUTypes::InstructionDef){};
    instructionTable[0xf4] = (CPUTypes::InstructionDef){};
    instructionTable[0xf5] = (CPUTypes::InstructionDef){};
    instructionTable[0xf6] = (CPUTypes::InstructionDef){};
    instructionTable[0xf7] = (CPUTypes::InstructionDef){};
    instructionTable[0xf8] = (CPUTypes::InstructionDef){};
    instructionTable[0xf9] = (CPUTypes::InstructionDef){};
    instructionTable[0xfa] = (CPUTypes::InstructionDef){};
    instructionTable[0xfb] = (CPUTypes::InstructionDef){};
    instructionTable[0xfc] = (CPUTypes::InstructionDef){};
    instructionTable[0xfd] = (CPUTypes::InstructionDef){};
    instructionTable[0xfe] = (CPUTypes::InstructionDef){};
    instructionTable[0xff] = (CPUTypes::InstructionDef){};

}


void CPU::printRegisters() {
    std::cout << "A " << std::hex << (unsigned)registers.A << '\n';
    std::cout << "X " << std::hex << (unsigned)registers.X << '\n';
    std::cout << "Y " << std::hex << (unsigned)registers.X << '\n';
    std::cout << "SP " << std::hex << (unsigned)registers.X << '\n';
    std::cout << "Status ";
    
    for(int i = 7; i >= 0; i--) {
        if(i == 3) {
            std::cout << ' ';
        }
        std::cout << ((registers.status & (1 << i)) >> i);
    }
    
    std::cout << '\n';
    std::cout << "PC " << (unsigned)registers.X << std::endl;
}

uint8_t CPU::getFlagValue(CPUTypes::Flags f) {
    return (registers.status & (1 << f)) >> f;
}

void CPU::setFlagValue(CPUTypes::Flags f, uint8_t b) {
    if(b == 0 || f == CPUTypes::Flags::ONE) {
        return;
    }

    registers.status |= b << f;
}


// Instructions 
// Access
void CPU::LDA() {}
void CPU::LDX() {}
void CPU::LDY() {}
void CPU::STA() {}
void CPU::STX() {}
void CPU::STY() {}

// Transfe
void CPU::TAX() {}
void CPU::TAY() {}
void CPU::TXA() {}
void CPU::TYA() {}

// Arithmeic
void CPU::ADC() {}
void CPU::DEC() {}
void CPU::DEX() {}
void CPU::DEY() {}
void CPU::INC() {}
void CPU::INX() {}
void CPU::INY() {}
void CPU::SBC() {}

// Shift
void CPU::ASL() {}
void CPU::LSR() {}
void CPU::ROL() {}
void CPU::ROR() {}

// Bitwise
void CPU::AND() {}
void CPU::BIT() {}
void CPU::EOR() {}
void CPU::ORA() {}

// Compare
void CPU::CMP() {}
void CPU::CPX() {}
void CPU::CPY() {}

// Branch
void CPU::BCC() {}
void CPU::BCS() {}
void CPU::BEQ() {}
void CPU::BMI() {}
void CPU::BNE() {}
void CPU::BPL() {}
void CPU::BVC() {}
void CPU::BVS() {}

// Jump
void CPU::BRK() {}
void CPU::JMP() {}
void CPU::JSR() {}
void CPU::RTI() {}
void CPU::RTS() {}

// Stack
void CPU::PHA() {}
void CPU::PHP() {}
void CPU::PLA() {}
void CPU::PLP() {}
void CPU::TSX() {}
void CPU::TXS() {}

// Flags
void CPU::CLC() {}
void CPU::CLD() {}
void CPU::CLI() {}
void CPU::CLV() {}
void CPU::SEC() {}
void CPU::SED() {}
void CPU::SEI() {}

// Other
void CPU::NOP() {}

