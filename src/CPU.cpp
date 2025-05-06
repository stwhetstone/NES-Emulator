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

    instructionTable[0x61] = {2, 6, std::bind(&CPU::ADC, this)};
    instructionTable[0x65] = {2, 3, std::bind(&CPU::ADC, this)};
    instructionTable[0x69] = {2, 2, std::bind(&CPU::ADC, this)};
    instructionTable[0x6D] = {3, 4, std::bind(&CPU::ADC, this)};
    instructionTable[0x71] = {2, 5, std::bind(&CPU::ADC, this)};
    instructionTable[0x75] = {2, 4, std::bind(&CPU::ADC, this)};
    instructionTable[0x79] = {3, 4, std::bind(&CPU::ADC, this)};
    instructionTable[0x7D] = {3, 4, std::bind(&CPU::ADC, this)};

    instructionTable[0x21] = {2, 6, std::bind(&CPU::AND, this)};
    instructionTable[0x25] = {2, 3, std::bind(&CPU::AND, this)};
    instructionTable[0x29] = {2, 2, std::bind(&CPU::AND, this)};
    instructionTable[0x2D] = {3, 4, std::bind(&CPU::AND, this)};
    instructionTable[0x31] = {2, 5, std::bind(&CPU::AND, this)};
    instructionTable[0x35] = {2, 4, std::bind(&CPU::AND, this)};
    instructionTable[0x39] = {3, 4, std::bind(&CPU::AND, this)};
    instructionTable[0x3D] = {3, 4, std::bind(&CPU::AND, this)};
    
    instructionTable[0x06] = {2, 5, std::bind(&CPU::ASL, this)};
    instructionTable[0x0A] = {1, 2, std::bind(&CPU::ASL, this)};
    instructionTable[0x0E] = {3, 6, std::bind(&CPU::ASL, this)};
    instructionTable[0x16] = {2, 6, std::bind(&CPU::ASL, this)};
    instructionTable[0x1E] = {3, 7, std::bind(&CPU::ASL, this)};
    
    instructionTable[0x90] = {2, 2, std::bind(&CPU::BCC, this)};
    
    instructionTable[0xB0] = {2, 2, std::bind(&CPU::BCS, this)};
    
    instructionTable[0xF0] = {2, 2, std::bind(&CPU::BEQ, this)};
    
    instructionTable[0x24] = {2, 3, std::bind(&CPU::BIT, this)};
    instructionTable[0x2C] = {3, 4, std::bind(&CPU::BIT, this)};
    
    instructionTable[0x30] = {2, 2, std::bind(&CPU::BMI, this)};
    
    instructionTable[0xD0] = {2, 2, std::bind(&CPU::BNE, this)};
    
    instructionTable[0x10] = {2, 2, std::bind(&CPU::BPL, this)};
    
    instructionTable[0x00] = {1, 7, std::bind(&CPU::BRK, this)};
    
    instructionTable[0x50] = {2, 2, std::bind(&CPU::BVC, this)};
    
    instructionTable[0x70] = {2, 2, std::bind(&CPU::BVS, this)};
    
    instructionTable[0x18] = {1, 2, std::bind(&CPU::CLC, this)};
    
    instructionTable[0xD8] = {1, 2, std::bind(&CPU::CLD, this)};
    
    instructionTable[0x58] = {1, 2, std::bind(&CPU::CLI, this)};
    
    instructionTable[0xB8] = {1, 2, std::bind(&CPU::CLV, this)};
    
    instructionTable[0xC1] = {2, 6, std::bind(&CPU::CMP, this)};
    instructionTable[0xC5] = {2, 3, std::bind(&CPU::CMP, this)};
    instructionTable[0xC9] = {2, 2, std::bind(&CPU::CMP, this)};
    instructionTable[0xCD] = {3, 4, std::bind(&CPU::CMP, this)};
    instructionTable[0xD1] = {2, 5, std::bind(&CPU::CMP, this)};
    instructionTable[0xD5] = {2, 4, std::bind(&CPU::CMP, this)};
    instructionTable[0xD9] = {3, 4, std::bind(&CPU::CMP, this)};
    instructionTable[0xDD] = {3, 4, std::bind(&CPU::CMP, this)};
    
    instructionTable[0xE0] = {2, 2, std::bind(&CPU::CPX, this)};
    instructionTable[0xE4] = {2, 3, std::bind(&CPU::CPX, this)};
    instructionTable[0xEC] = {3, 4, std::bind(&CPU::CPX, this)};

    instructionTable[0xC0] = {2, 2, std::bind(&CPU::CPY, this)};
    instructionTable[0xC4] = {2, 3, std::bind(&CPU::CPY, this)};
    instructionTable[0xCC] = {3, 4, std::bind(&CPU::CPY, this)};
    
    instructionTable[0xC6] = {2, 5, std::bind(&CPU::DEC, this)};
    instructionTable[0xCE] = {3, 6, std::bind(&CPU::DEC, this)};
    instructionTable[0xD6] = {2, 6, std::bind(&CPU::DEC, this)};
    instructionTable[0xDE] = {3, 7, std::bind(&CPU::DEC, this)};
    
    instructionTable[0xCA] = {1, 2, std::bind(&CPU::DEX, this)};
    
    instructionTable[0x88] = {1, 2, std::bind(&CPU::DEY, this)};
    
    instructionTable[0x41] = {2, 6, std::bind(&CPU::EOR, this)};
    instructionTable[0x45] = {2, 3, std::bind(&CPU::EOR, this)};
    instructionTable[0x49] = {2, 2, std::bind(&CPU::EOR, this)};
    instructionTable[0x4D] = {3, 4, std::bind(&CPU::EOR, this)};
    instructionTable[0x51] = {2, 5, std::bind(&CPU::EOR, this)};
    instructionTable[0x55] = {2, 4, std::bind(&CPU::EOR, this)};
    instructionTable[0x59] = {3, 4, std::bind(&CPU::EOR, this)};
    instructionTable[0x5D] = {3, 4, std::bind(&CPU::EOR, this)};
    
    instructionTable[0xE6] = {2, 5, std::bind(&CPU::INC, this)};
    instructionTable[0xEE] = {3, 6, std::bind(&CPU::INC, this)};
    instructionTable[0xF6] = {2, 6, std::bind(&CPU::INC, this)};
    instructionTable[0xFE] = {3, 7, std::bind(&CPU::INC, this)};
    
    instructionTable[0xE8] = {1, 2, std::bind(&CPU::INX, this)};
    
    instructionTable[0xC8] = {1, 2, std::bind(&CPU::INY, this)};
    
    instructionTable[0x4C] = {3, 3, std::bind(&CPU::JMP, this)};
    instructionTable[0x6C] = {3, 5, std::bind(&CPU::JMP, this)};
    
    instructionTable[0x20] = {3, 6, std::bind(&CPU::JSR, this)};
    
    instructionTable[0xA1] = {2, 6, std::bind(&CPU::LDA, this)};
    instructionTable[0xA5] = {2, 3, std::bind(&CPU::LDA, this)};
    instructionTable[0xA9] = {2, 2, std::bind(&CPU::LDA, this)};
    instructionTable[0xAD] = {3, 4, std::bind(&CPU::LDA, this)};
    instructionTable[0xB1] = {2, 5, std::bind(&CPU::LDA, this)};
    instructionTable[0xB5] = {2, 4, std::bind(&CPU::LDA, this)};
    instructionTable[0xB9] = {3, 4, std::bind(&CPU::LDA, this)};
    instructionTable[0xBD] = {3, 4, std::bind(&CPU::LDA, this)};
    
    instructionTable[0xA2] = {2, 2, std::bind(&CPU::LDX, this)};
    instructionTable[0xA6] = {2, 3, std::bind(&CPU::LDX, this)};
    instructionTable[0xAE] = {3, 4, std::bind(&CPU::LDX, this)};
    instructionTable[0xB6] = {2, 4, std::bind(&CPU::LDX, this)};
    instructionTable[0xBE] = {3, 4, std::bind(&CPU::LDX, this)};
    
    instructionTable[0xA0] = {2, 2, std::bind(&CPU::LDY, this)};
    instructionTable[0xA4] = {2, 3, std::bind(&CPU::LDY, this)};
    instructionTable[0xAC] = {3, 4, std::bind(&CPU::LDY, this)};
    instructionTable[0xB4] = {2, 4, std::bind(&CPU::LDY, this)};
    instructionTable[0xBC] = {3, 4, std::bind(&CPU::LDY, this)};
    
    instructionTable[0x46] = {2, 5, std::bind(&CPU::LSR, this)};
    instructionTable[0x4A] = {1, 2, std::bind(&CPU::LSR, this)};
    instructionTable[0x4E] = {3, 6, std::bind(&CPU::LSR, this)};
    instructionTable[0x56] = {2, 6, std::bind(&CPU::LSR, this)};
    instructionTable[0x5E] = {3, 7, std::bind(&CPU::LSR, this)};
    
    instructionTable[0xEA] = {1, 2, std::bind(&CPU::NOP, this)};

    instructionTable[0x01] = {2, 6, std::bind(&CPU::ORA, this)};
    instructionTable[0x05] = {2, 3, std::bind(&CPU::ORA, this)};
    instructionTable[0x09] = {2, 2, std::bind(&CPU::ORA, this)};
    instructionTable[0x0D] = {3, 4, std::bind(&CPU::ORA, this)};
    instructionTable[0x11] = {2, 5, std::bind(&CPU::ORA, this)};
    instructionTable[0x15] = {2, 4, std::bind(&CPU::ORA, this)};
    instructionTable[0x19] = {3, 4, std::bind(&CPU::ORA, this)};
    instructionTable[0x1D] = {3, 4, std::bind(&CPU::ORA, this)};

    instructionTable[0x48] = {1, 3, std::bind(&CPU::PHA, this)};
    
    instructionTable[0x08] = {1, 3, std::bind(&CPU::PHP, this)};
    
    instructionTable[0x68] = {1, 4, std::bind(&CPU::PLA, this)};
    
    instructionTable[0x28] = {1, 4, std::bind(&CPU::PLP, this)};
    
    instructionTable[0x26] = {2, 5, std::bind(&CPU::ROL, this)};
    instructionTable[0x2A] = {1, 2, std::bind(&CPU::ROL, this)};
    instructionTable[0x2E] = {3, 6, std::bind(&CPU::ROL, this)};
    instructionTable[0x36] = {2, 6, std::bind(&CPU::ROL, this)};
    instructionTable[0x3E] = {3, 7, std::bind(&CPU::ROL, this)};
    
    instructionTable[0x66] = {2, 5, std::bind(&CPU::ROR, this)};
    instructionTable[0x6A] = {1, 2, std::bind(&CPU::ROR, this)};
    instructionTable[0x6E] = {3, 6, std::bind(&CPU::ROR, this)};
    instructionTable[0x76] = {2, 6, std::bind(&CPU::ROR, this)};
    instructionTable[0x7E] = {3, 7, std::bind(&CPU::ROR, this)};
    
    instructionTable[0x40] = {1, 6, std::bind(&CPU::RTI, this)};
    
    instructionTable[0x60] = {1, 6, std::bind(&CPU::RTS, this)};
    
    instructionTable[0x38] = {1, 2, std::bind(&CPU::SEC, this)};
    instructionTable[0xE1] = {2, 6, std::bind(&CPU::SBC, this)};
    instructionTable[0xE5] = {2, 3, std::bind(&CPU::SBC, this)};
    instructionTable[0xE9] = {2, 2, std::bind(&CPU::SBC, this)};
    instructionTable[0xED] = {3, 4, std::bind(&CPU::SBC, this)};
    instructionTable[0xF1] = {2, 5, std::bind(&CPU::SBC, this)};
    instructionTable[0xF5] = {2, 4, std::bind(&CPU::SBC, this)};
    instructionTable[0xF9] = {3, 4, std::bind(&CPU::SBC, this)};
    instructionTable[0xFD] = {3, 4, std::bind(&CPU::SBC, this)};
    
    instructionTable[0xF8] = {1, 2, std::bind(&CPU::SED, this)};
    
    instructionTable[0x78] = {1, 2, std::bind(&CPU::SEI, this)};
    
    instructionTable[0x81] = {2, 6, std::bind(&CPU::STA, this)};
    instructionTable[0x85] = {2, 3, std::bind(&CPU::STA, this)};
    instructionTable[0x8d] = {3, 4, std::bind(&CPU::STA, this)};
    instructionTable[0x91] = {2, 6, std::bind(&CPU::STA, this)};
    instructionTable[0x95] = {2, 4, std::bind(&CPU::STA, this)};
    instructionTable[0x99] = {3, 5, std::bind(&CPU::STA, this)};
    instructionTable[0x9d] = {3, 5, std::bind(&CPU::STA, this)};
    
    instructionTable[0x86] = {2, 3, std::bind(&CPU::STX, this)};
    instructionTable[0x8e] = {3, 4, std::bind(&CPU::STX, this)};
    instructionTable[0x96] = {2, 4, std::bind(&CPU::STX, this)};
    
    instructionTable[0x84] = {2, 3, std::bind(&CPU::STY, this)};
    instructionTable[0x8c] = {3, 4, std::bind(&CPU::STY, this)};
    instructionTable[0x94] = {2, 4, std::bind(&CPU::STY, this)};
    
    instructionTable[0xAA] = {1, 2, std::bind(&CPU::TAX, this)};
    
    instructionTable[0xa8] = {1, 2, std::bind(&CPU::TAY, this)};
    
    instructionTable[0xba] = {1, 2, std::bind(&CPU::TSX, this)};
    
    instructionTable[0x8a] = {1, 2, std::bind(&CPU::TXA, this)};
    
    instructionTable[0x9A] = {1, 2, std::bind(&CPU::TXS, this)};
    
    instructionTable[0x98] = {1, 2, std::bind(&CPU::TYA, this)};
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
void CPU::BRK() {
    std::cout << "brk" << std::endl;
}

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



