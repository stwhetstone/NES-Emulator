#include <iostream>

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
 
    cyclesRemaining = 0;

    initInstructionTable();
}

void CPU::initInstructionTable() {
    instructionTable.reserve(0xff);

    // lambda because std::bind is slow and to do void(CPU::*fnc), instructiondefs would have to be in CPU.hpp
    auto adcl = [this](){this->ADC();};
    instructionTable[0x69] = {2, 2, adcl};
    instructionTable[0x65] = {2, 3, adcl};
    instructionTable[0x75] = {2, 4, adcl};
    instructionTable[0x6D] = {3, 4, adcl};
    instructionTable[0x7D] = {3, 4, adcl};
    instructionTable[0x79] = {3, 4, adcl};
    instructionTable[0x61] = {2, 6, adcl};
    instructionTable[0x71] = {2, 5, adcl};

    auto andl = [this](){this->AND();};
    instructionTable[0x29] = {2, 2, andl};
    instructionTable[0x25] = {2, 3, andl};
    instructionTable[0x35] = {2, 4, andl};
    instructionTable[0x2D] = {3, 4, andl};
    instructionTable[0x3D] = {3, 4, andl};
    instructionTable[0x39] = {3, 4, andl};
    instructionTable[0x21] = {2, 6, andl};
    instructionTable[0x31] = {2, 5, andl};

    auto asll = [this](){this->ASL();};
    instructionTable[0x0A] = {1, 2, asll};
    instructionTable[0x06] = {2, 5, asll};
    instructionTable[0x16] = {2, 6, asll};
    instructionTable[0x0E] = {3, 6, asll};
    instructionTable[0x1E] = {3, 7, asll};

    auto bccl = [this](){this->BCC();};
    instructionTable[0x90] = {2, 2, bccl};

    auto bcsl = [this](){this->BCS();};
    instructionTable[0xB0] = {2, 2, bcsl};

    auto beql = [this](){this->BEQ();};
    instructionTable[0xF0] = {2, 2, beql};

    auto bitl = [this](){this->BIT();};
    instructionTable[0x24] = {2, 3, bitl};
    instructionTable[0x2C] = {3, 4, bitl};

    auto bmil = [this](){this->BMI();};
    instructionTable[0x30] = {2, 2, bmil};

    auto bnel = [this](){this->BNE();};
    instructionTable[0xD0] = {2, 2, bnel};

    auto bpll = [this](){this->BPL();};
    instructionTable[0x10] = {2, 2, bpll};

    auto brkl = [this](){this->BRK();};
    instructionTable[0x00] = {1, 7, brkl};

    auto bvcl = [this](){this->BVC();};
    instructionTable[0x50] = {2, 2, bvcl};

    auto bvsl = [this](){this->BVS();};
    instructionTable[0x70] = {2, 2, bvsl};

    auto clcl = [this](){this->CLC();};
    instructionTable[0x18] = {1, 2, clcl};

    auto cldl = [this](){this->CLD();};
    instructionTable[0xD8] = {1, 2, cldl};

    auto clil = [this](){this->CLI();};
    instructionTable[0x58] = {1, 2, clil};

    auto clvl = [this](){this->CLV();};
    instructionTable[0xB8] = {1, 2, clvl};

    auto cmpl = [this](){this->CMP();};
    instructionTable[0xC9] = {2, 2, cmpl};
    instructionTable[0xC5] = {2, 3, cmpl};
    instructionTable[0xD5] = {2, 4, cmpl};
    instructionTable[0xCD] = {3, 4, cmpl};
    instructionTable[0xDD] = {3, 4, cmpl};
    instructionTable[0xD9] = {3, 4, cmpl};
    instructionTable[0xC1] = {2, 6, cmpl};
    instructionTable[0xD1] = {2, 5, cmpl};

    auto cpxl = [this](){this->CPX();};
    instructionTable[0xE0] = {2, 2, cpxl};
    instructionTable[0xE4] = {2, 3, cpxl};
    instructionTable[0xEC] = {3, 4, cpxl};

    auto cpyl = [this](){this->CPY();};
    instructionTable[0xC0] = {2, 2, cpyl};
    instructionTable[0xC4] = {2, 3, cpyl};
    instructionTable[0xCC] = {3, 4, cpyl};

    auto decl = [this](){this->DEC();};
    instructionTable[0xC6] = {2, 5, decl};
    instructionTable[0xD6] = {2, 6, decl};
    instructionTable[0xCE] = {3, 6, decl};
    instructionTable[0xDE] = {3, 7, decl};

    auto dexl = [this](){this->DEX();};
    instructionTable[0xCA] = {1, 2, dexl};

    auto deyl = [this](){this->DEY();};
    instructionTable[0x88] = {1, 2, deyl};

    auto eorl = [this](){this->EOR();};
    instructionTable[0x49] = {2, 2, eorl};
    instructionTable[0x45] = {2, 3, eorl};
    instructionTable[0x55] = {2, 4, eorl};
    instructionTable[0x4D] = {3, 4, eorl};
    instructionTable[0x5D] = {3, 4, eorl};
    instructionTable[0x59] = {3, 4, eorl};
    instructionTable[0x41] = {2, 6, eorl};
    instructionTable[0x51] = {2, 5, eorl};

    auto incl = [this](){this->INC();};
    instructionTable[0xE6] = {2, 5, incl};
    instructionTable[0xF6] = {2, 6, incl};
    instructionTable[0xEE] = {3, 6, incl};
    instructionTable[0xFE] = {3, 7, incl};

    auto inxl = [this](){this->INX();};
    instructionTable[0xE8] = {1, 2, inxl};

    auto inyl = [this](){this->INY();};
    instructionTable[0xC8] = {1, 2, inyl};

    auto jmpl = [this](){this->JMP();};
    instructionTable[0x4C] = {3, 3, jmpl};
    instructionTable[0x6C] = {3, 5, jmpl};

    auto jsrl = [this](){this->JSR();};
    instructionTable[0x20] = {3, 6, jsrl};

    auto ldal = [this](){this->LDA();};
    instructionTable[0xA9] = {2, 2, ldal};
    instructionTable[0xA5] = {2, 3, ldal};
    instructionTable[0xB5] = {2, 4, ldal};
    instructionTable[0xAD] = {3, 4, ldal};
    instructionTable[0xBD] = {3, 4, ldal};
    instructionTable[0xB9] = {3, 4, ldal};
    instructionTable[0xA1] = {2, 6, ldal};
    instructionTable[0xB1] = {2, 5, ldal};

    auto ldxl = [this](){this->LDX();};
    instructionTable[0xA2] = {2, 2, ldxl};
    instructionTable[0xA6] = {2, 3, ldxl};
    instructionTable[0xB6] = {2, 4, ldxl};
    instructionTable[0xAE] = {3, 4, ldxl};
    instructionTable[0xBE] = {3, 4, ldxl};

    auto ldyl = [this](){this->LDY();};
    instructionTable[0xA0] = {2, 2, ldyl};
    instructionTable[0xA4] = {2, 3, ldyl};
    instructionTable[0xB4] = {2, 4, ldyl};
    instructionTable[0xAC] = {3, 4, ldyl};
    instructionTable[0xBC] = {3, 4, ldyl};

    auto lsrl = [this](){this->LSR();};
    instructionTable[0x4A] = {1, 2, lsrl};
    instructionTable[0x46] = {2, 5, lsrl};
    instructionTable[0x56] = {2, 6, lsrl};
    instructionTable[0x4E] = {3, 6, lsrl};
    instructionTable[0x5E] = {3, 7, lsrl};

    auto nopl = [this](){this->NOP();};
    instructionTable[0xEA] = {1, 2, nopl};

    auto oral = [this](){this->ORA();};
    instructionTable[0x09] = {2, 2, oral};
    instructionTable[0x05] = {2, 3, oral};
    instructionTable[0x15] = {2, 4, oral};
    instructionTable[0x0D] = {3, 4, oral};
    instructionTable[0x1D] = {3, 4, oral};
    instructionTable[0x19] = {3, 4, oral};
    instructionTable[0x01] = {2, 6, oral};
    instructionTable[0x11] = {2, 5, oral};

    auto phal = [this](){this->PHA();};
    instructionTable[0x48] = {1, 3, phal};

    auto phpl = [this](){this->PHP();};
    instructionTable[0x08] = {1, 3, phpl};

    auto plal = [this](){this->PLA();};
    instructionTable[0x68] = {1, 4, plal};

    auto plpl = [this](){this->PLP();};
    instructionTable[0x28] = {1, 4, plpl};

    auto roll = [this](){this->ROL();};
    instructionTable[0x2A] = {1, 2, roll};
    instructionTable[0x26] = {2, 5, roll};
    instructionTable[0x36] = {2, 6, roll};
    instructionTable[0x2E] = {3, 6, roll};
    instructionTable[0x3E] = {3, 7, roll};

    auto rorl = [this](){this->ROR();};
    instructionTable[0x6A] = {1, 2, rorl};
    instructionTable[0x66] = {2, 5, rorl};
    instructionTable[0x76] = {2, 6, rorl};
    instructionTable[0x6E] = {3, 6, rorl};
    instructionTable[0x7E] = {3, 7, rorl};

    auto rtil = [this](){this->RTI();};
    instructionTable[0x40] = {1, 6, rtil};

    auto rtsl = [this](){this->RTS();};
    instructionTable[0x60] = {1, 6, rtsl};

    auto sbcl = [this](){this->SBC();};
    instructionTable[0xE9] = {2, 2, sbcl};
    instructionTable[0xE5] = {2, 3, sbcl};
    instructionTable[0xF5] = {2, 4, sbcl};
    instructionTable[0xED] = {3, 4, sbcl};
    instructionTable[0xFD] = {3, 4, sbcl};
    instructionTable[0xF9] = {3, 4, sbcl};
    instructionTable[0xE1] = {2, 6, sbcl};
    instructionTable[0xF1] = {2, 5, sbcl};

    auto secl = [this](){this->SEC();};
    instructionTable[0x38] = {1, 2, secl};

    auto sedl = [this](){this->SED();};
    instructionTable[0xF8] = {1, 2, sedl};

    auto seil = [this](){this->SEI();};
    instructionTable[0x78] = {1, 2, seil};

    auto stal = [this](){this->STA();};
    instructionTable[0x85] = {2, 3, stal};
    instructionTable[0x95] = {2, 4, stal};
    instructionTable[0x8d] = {3, 4, stal};
    instructionTable[0x9d] = {3, 5, stal};
    instructionTable[0x99] = {3, 5, stal};
    instructionTable[0x81] = {2, 6, stal};
    instructionTable[0x91] = {2, 6, stal};

    auto stxl = [this](){this->STX();};
    instructionTable[0x86] = {2, 3, stxl};
    instructionTable[0x96] = {2, 4, stxl};
    instructionTable[0x8e] = {3, 4, stxl};

    auto styl = [this](){this->STY();};
    instructionTable[0x84] = {2, 3, styl};
    instructionTable[0x94] = {2, 4, styl};
    instructionTable[0x8c] = {3, 4, styl};

    auto taxl = [this](){this->TAX();};
    instructionTable[0xAA] = {1, 2, taxl};

    auto tayl = [this](){this->TAY();};
    instructionTable[0xa8] = {1, 2, tayl};

    auto tsxl = [this](){this->TSX();};
    instructionTable[0xba] = {1, 2, tsxl};

    auto txal = [this](){this->TXA();};
    instructionTable[0x8a] = {1, 2, txal};

    auto txsl = [this](){this->TXS();};
    instructionTable[0x9A] = {1, 2, txsl};

    auto tyal = [this](){this->TYA();};
    instructionTable[0x98] = {1, 2, tyal};
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



