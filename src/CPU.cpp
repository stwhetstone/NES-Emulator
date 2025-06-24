#include <iostream>

#include "CPU.hpp"


CPU::CPU(NESTypes::Bus &b) : bus(b) {
    registers = {};

    registers.A = 0;
    registers.X = 0;
    registers.Y = 0;

    registers.SP = 0xff;

    // NV1B DIZC
    registers.status = 0b00100100;

    registers.PC = 0xfffc;

    resetVector[0] = -1;
    resetVector[1] = -1;
    cyclesRemaining = 0;

    initInstructionTable();
}


void CPU::initInstructionTable() {
    instructionTable.reserve(0xff);

    // lambda because std::bind is slow and to do void(CPU::*fnc), instructiondefs would have to be in CPU.hpp
    auto adcl = [this](){this->ADC();};
    instructionTable[0x69] = {2, 2, 1, adcl};
    instructionTable[0x65] = {2, 3, 1, adcl};
    instructionTable[0x75] = {2, 4, 1, adcl};
    instructionTable[0x6d] = {3, 4, 1, adcl};
    instructionTable[0x7d] = {3, 4, 1, adcl};
    instructionTable[0x79] = {3, 4, 1, adcl};
    instructionTable[0x61] = {2, 6, 1, adcl};
    instructionTable[0x71] = {2, 5, 1, adcl};

    auto andl = [this](){this->AND();};
    instructionTable[0x29] = {2, 2, 1, andl};
    instructionTable[0x25] = {2, 3, 1, andl};
    instructionTable[0x35] = {2, 4, 1, andl};
    instructionTable[0x2d] = {3, 4, 1, andl};
    instructionTable[0x3d] = {3, 4, 1, andl};
    instructionTable[0x39] = {3, 4, 1, andl};
    instructionTable[0x21] = {2, 6, 1, andl};
    instructionTable[0x31] = {2, 5, 1, andl};

    auto asll = [this](){this->ASL();};
    instructionTable[0x0a] = {1, 2, 1, asll};
    instructionTable[0x06] = {2, 5, 1, asll};
    instructionTable[0x16] = {2, 6, 1, asll};
    instructionTable[0x0e] = {3, 6, 1, asll};
    instructionTable[0x1e] = {3, 7, 1, asll};

    auto bccl = [this](){this->BCC();};
    instructionTable[0x90] = {2, 2, 1, bccl};

    auto bcsl = [this](){this->BCS();};
    instructionTable[0xB0] = {2, 2, 1, bcsl};

    auto beql = [this](){this->BEQ();};
    instructionTable[0xF0] = {2, 2, 1, beql};

    auto bitl = [this](){this->BIT();};
    instructionTable[0x24] = {2, 3, 1, bitl};
    instructionTable[0x2c] = {3, 4, 1, bitl};

    auto bmil = [this](){this->BMI();};
    instructionTable[0x30] = {2, 2, 1, bmil};

    auto bnel = [this](){this->BNE();};
    instructionTable[0xd0] = {2, 2, 1, bnel};

    auto bpll = [this](){this->BPL();};
    instructionTable[0x10] = {2, 2, 1, bpll};

    auto brkl = [this](){this->BRK();};
    instructionTable[0x00] = {1, 7, 1, brkl};

    auto bvcl = [this](){this->BVC();};
    instructionTable[0x50] = {2, 2, 1, bvcl};

    auto bvsl = [this](){this->BVS();};
    instructionTable[0x70] = {2, 2, 1, bvsl};

    auto clcl = [this](){this->CLC();};
    instructionTable[0x18] = {1, 2, 1, clcl};

    auto cldl = [this](){this->CLD();};
    instructionTable[0xd8] = {1, 2, 1, cldl};

    auto clil = [this](){this->CLI();};
    instructionTable[0x58] = {1, 2, 1, clil};

    auto clvl = [this](){this->CLV();};
    instructionTable[0xb8] = {1, 2, 1, clvl};

    auto cmpl = [this](){this->CMP();};
    instructionTable[0xc9] = {2, 2, 1, cmpl};
    instructionTable[0xc5] = {2, 3, 1, cmpl};
    instructionTable[0xd5] = {2, 4, 1, cmpl};
    instructionTable[0xcd] = {3, 4, 1, cmpl};
    instructionTable[0xdd] = {3, 4, 1, cmpl};
    instructionTable[0xd9] = {3, 4, 1, cmpl};
    instructionTable[0xc1] = {2, 6, 1, cmpl};
    instructionTable[0xd1] = {2, 5, 1, cmpl};

    auto cpxl = [this](){this->CPX();};
    instructionTable[0xe0] = {2, 2, 1, cpxl};
    instructionTable[0xe4] = {2, 3, 1, cpxl};
    instructionTable[0xec] = {3, 4, 1, cpxl};

    auto cpyl = [this](){this->CPY();};
    instructionTable[0xc0] = {2, 2, 1, cpyl};
    instructionTable[0xc4] = {2, 3, 1, cpyl};
    instructionTable[0xcc] = {3, 4, 1, cpyl};

    auto decl = [this](){this->DEC();};
    instructionTable[0xc6] = {2, 5, 1, decl};
    instructionTable[0xd6] = {2, 6, 1, decl};
    instructionTable[0xce] = {3, 6, 1, decl};
    instructionTable[0xde] = {3, 7, 1, decl};

    auto dexl = [this](){this->DEX();};
    instructionTable[0xca] = {1, 2, 1, dexl};

    auto deyl = [this](){this->DEY();};
    instructionTable[0x88] = {1, 2, 1, deyl};

    auto eorl = [this](){this->EOR();};
    instructionTable[0x49] = {2, 2, 1, eorl};
    instructionTable[0x45] = {2, 3, 1, eorl};
    instructionTable[0x55] = {2, 4, 1, eorl};
    instructionTable[0x4d] = {3, 4, 1, eorl};
    instructionTable[0x5d] = {3, 4, 1, eorl};
    instructionTable[0x59] = {3, 4, 1, eorl};
    instructionTable[0x41] = {2, 6, 1, eorl};
    instructionTable[0x51] = {2, 5, 1, eorl};

    auto incl = [this](){this->INC();};
    instructionTable[0xe6] = {2, 5, 1, incl};
    instructionTable[0xf6] = {2, 6, 1, incl};
    instructionTable[0xee] = {3, 6, 1, incl};
    instructionTable[0xfe] = {3, 7, 1, incl};

    auto inxl = [this](){this->INX();};
    instructionTable[0xe8] = {1, 2, 1, inxl};

    auto inyl = [this](){this->INY();};
    instructionTable[0xc8] = {1, 2, 1, inyl};

    auto jmpl = [this](){this->JMP();};
    instructionTable[0x4c] = {3, 3, 1, jmpl};
    instructionTable[0x6c] = {3, 5, 1, jmpl};

    auto jsrl = [this](){this->JSR();};
    instructionTable[0x20] = {3, 6, 1, jsrl};

    auto ldal = [this](){this->LDA();};
    instructionTable[0xa9] = {2, 2, 1, ldal};
    instructionTable[0xa5] = {2, 3, 1, ldal};
    instructionTable[0xb5] = {2, 4, 1, ldal};
    instructionTable[0xad] = {3, 4, 1, ldal};
    instructionTable[0xbd] = {3, 4, 1, ldal};
    instructionTable[0xb9] = {3, 4, 1, ldal};
    instructionTable[0xa1] = {2, 6, 1, ldal};
    instructionTable[0xb1] = {2, 5, 1, ldal};

    auto ldxl = [this](){this->LDX();};
    instructionTable[0xa2] = {2, 2, 1, ldxl};
    instructionTable[0xa6] = {2, 3, 1, ldxl};
    instructionTable[0xb6] = {2, 4, 1, ldxl};
    instructionTable[0xae] = {3, 4, 1, ldxl};
    instructionTable[0xbe] = {3, 4, 1, ldxl};

    auto ldyl = [this](){this->LDY();};
    instructionTable[0xa0] = {2, 2, 1, ldyl};
    instructionTable[0xa4] = {2, 3, 1, ldyl};
    instructionTable[0xb4] = {2, 4, 1, ldyl};
    instructionTable[0xac] = {3, 4, 1, ldyl};
    instructionTable[0xbc] = {3, 4, 1, ldyl};

    auto lsrl = [this](){this->LSR();};
    instructionTable[0x4a] = {1, 2, 1, lsrl};
    instructionTable[0x46] = {2, 5, 1, lsrl};
    instructionTable[0x56] = {2, 6, 1, lsrl};
    instructionTable[0x4e] = {3, 6, 1, lsrl};
    instructionTable[0x5e] = {3, 7, 1, lsrl};

    auto nopl = [this](){this->NOP();};
    instructionTable[0xea] = {1, 2, 1, nopl};

    auto oral = [this](){this->ORA();};
    instructionTable[0x09] = {2, 2, 1, oral};
    instructionTable[0x05] = {2, 3, 1, oral};
    instructionTable[0x15] = {2, 4, 1, oral};
    instructionTable[0x0d] = {3, 4, 1, oral};
    instructionTable[0x1d] = {3, 4, 1, oral};
    instructionTable[0x19] = {3, 4, 1, oral};
    instructionTable[0x01] = {2, 6, 1, oral};
    instructionTable[0x11] = {2, 5, 1, oral};

    auto phal = [this](){this->PHA();};
    instructionTable[0x48] = {1, 3, 1, phal};

    auto phpl = [this](){this->PHP();};
    instructionTable[0x08] = {1, 3, 1, phpl};

    auto plal = [this](){this->PLA();};
    instructionTable[0x68] = {1, 4, 1, plal};

    auto plpl = [this](){this->PLP();};
    instructionTable[0x28] = {1, 4, 1, plpl};

    auto roll = [this](){this->ROL();};
    instructionTable[0x2a] = {1, 2, 1, roll};
    instructionTable[0x26] = {2, 5, 1, roll};
    instructionTable[0x36] = {2, 6, 1, roll};
    instructionTable[0x2e] = {3, 6, 1, roll};
    instructionTable[0x3e] = {3, 7, 1, roll};

    auto rorl = [this](){this->ROR();};
    instructionTable[0x6a] = {1, 2, 1, rorl};
    instructionTable[0x66] = {2, 5, 1, rorl};
    instructionTable[0x76] = {2, 6, 1, rorl};
    instructionTable[0x6e] = {3, 6, 1, rorl};
    instructionTable[0x7e] = {3, 7, 1, rorl};

    auto rtil = [this](){this->RTI();};
    instructionTable[0x40] = {1, 6, 1, rtil};

    auto rtsl = [this](){this->RTS();};
    instructionTable[0x60] = {1, 6, 1, rtsl};

    auto sbcl = [this](){this->SBC();};
    instructionTable[0xe9] = {2, 2, 1, sbcl};
    instructionTable[0xe5] = {2, 3, 1, sbcl};
    instructionTable[0xf5] = {2, 4, 1, sbcl};
    instructionTable[0xed] = {3, 4, 1, sbcl};
    instructionTable[0xfd] = {3, 4, 1, sbcl};
    instructionTable[0xf9] = {3, 4, 1, sbcl};
    instructionTable[0xe1] = {2, 6, 1, sbcl};
    instructionTable[0xf1] = {2, 5, 1, sbcl};

    auto secl = [this](){this->SEC();};
    instructionTable[0x38] = {1, 2, 1, secl};

    auto sedl = [this](){this->SED();};
    instructionTable[0xf8] = {1, 2, 1, sedl};

    auto seil = [this](){this->SEI();};
    instructionTable[0x78] = {1, 2, 1, seil};

    auto stal = [this](){this->STA();};
    instructionTable[0x85] = {2, 3, 1, stal};
    instructionTable[0x95] = {2, 4, 1, stal};
    instructionTable[0x8d] = {3, 4, 1, stal};
    instructionTable[0x9d] = {3, 5, 1, stal};
    instructionTable[0x99] = {3, 5, 1, stal};
    instructionTable[0x81] = {2, 6, 1, stal};
    instructionTable[0x91] = {2, 6, 1, stal};

    auto stxl = [this](){this->STX();};
    instructionTable[0x86] = {2, 3, 1, stxl};
    instructionTable[0x96] = {2, 4, 1, stxl};
    instructionTable[0x8e] = {3, 4, 1, stxl};

    auto styl = [this](){this->STY();};
    instructionTable[0x84] = {2, 3, 1, styl};
    instructionTable[0x94] = {2, 4, 1, styl};
    instructionTable[0x8c] = {3, 4, 1, styl};

    auto taxl = [this](){this->TAX();};
    instructionTable[0xaa] = {1, 2, 1, taxl};

    auto tayl = [this](){this->TAY();};
    instructionTable[0xa8] = {1, 2, 1, tayl};

    auto tsxl = [this](){this->TSX();};
    instructionTable[0xba] = {1, 2, 1, tsxl};

    auto txal = [this](){this->TXA();};
    instructionTable[0x8a] = {1, 2, 1, txal};

    auto txsl = [this](){this->TXS();};
    instructionTable[0x9a] = {1, 2, 1, txsl};

    auto tyal = [this](){this->TYA();};
    instructionTable[0x98] = {1, 2, 1, tyal};
}


uint8_t CPU::getFlagValue(CPUTypes::Flag f) {
    return (registers.status & (1 << f)) >> f;
}


void CPU::setStatusFlagValue(CPUTypes::Flag f, bool set) {
    if(f == CPUTypes::Flag::ONE) {
        return;
    }

    if(set) {
        registers.status |= 1 << f;
    } else if(!set) {
        registers.status &= ~(1 << f);
    }
}


void CPU::setStatusFlags(uint8_t flags, bool set) {
    // carry
    if((flags & 1) == 1) {
        setStatusFlagValue(CPUTypes::Flag::C, set);
    } 
    // zero
    if((flags & 2) >> 1 == 1) {
        setStatusFlagValue(CPUTypes::Flag::Z, set);
    } 
    // interrupt disable
    if((flags & 4) >> 2 == 1) {
        setStatusFlagValue(CPUTypes::Flag::I, set);
    } 
    // decimal
    if((flags & 8) >> 3 == 1) {
        setStatusFlagValue(CPUTypes::Flag::D, set);
    } 
    // b flag
    if((flags & 16) >> 4 == 1) {
        setStatusFlagValue(CPUTypes::Flag::B, set);
    } 
    // overflow 
    if((flags & 64) >> 6 == 1) {
        setStatusFlagValue(CPUTypes::Flag::V, set);
    } 
    // negative
    if((flags & 128) >> 7 == 1) {
        setStatusFlagValue(CPUTypes::Flag::N, set);
    } 
}


void CPU::executeInstruction() {
    uint8_t opcode = instruction[0];
    uint8_t size = instructionTable[opcode].size, cycles = instructionTable[opcode].cycles;

    if(cycles > size) {
        cyclesRemaining = cycles - size;
    }

    instructionTable[opcode].fnc();
}


void CPU::flattenInstructionArgument() {
    instruction[1] = ((instruction[2] << 8) | instruction[1]);
    instruction[2] = 0;
}


void CPU::incrementPC() {
    registers.PC++;
}


void CPU::printRegisters() {
    std::cout << "A " << std::hex << (unsigned)registers.A << '\n';
    std::cout << "X " << std::hex << (unsigned)registers.X << '\n';
    std::cout << "Y " << std::hex << (unsigned)registers.Y << '\n';
    std::cout << "SP " << std::hex << (unsigned)registers.SP << '\n';
    std::cout << "Status ";
    
    for(int i = 7; i >= 0; i--) {
        if(i == 3) {
            std::cout << ' ';
        }
        std::cout << ((registers.status & (1 << i)) >> i);
    }
    
    std::cout << '\n';
    std::cout << "PC " << std::hex << (unsigned)registers.PC << std::endl;
}


void CPU::aBusLoadInstructionArgument() {
    bus.address = instruction[1];
}


void CPU::aBusLoadPC() {
    bus.address = registers.PC;
}


void CPU::aBusStoreResetVector() {
    if(registers.PC != 0xfffc) {
        resetVector[0] = bus.data;
        return;
    }

    resetVector[1] = bus.data;
}


void CPU::dBusLoadInstructionArgument() {
    bus.data = instruction[1];
}


void CPU::dBusLoadRegister(CPUTypes::RegisterName r) {
    switch(r) {
        case CPUTypes::RegisterName::A:
            bus.data = registers.A;
            break;
        case CPUTypes::RegisterName::X:
            bus.data = registers.X;
            break;
        case CPUTypes::RegisterName::Y:
            bus.data = registers.Y;
            break;
        case CPUTypes::RegisterName::SP:
            bus.data = registers.SP;
            break;
        default:
            break;
    }
}


void CPU::dBusStoreInstruction(int i) {
    instruction[i] = bus.data;
}

void CPU::rwBusSetSignal() {
    uint8_t opcode = instruction[0];
    bus.rwSignal = instructionTable[opcode].rw;
}


// Instructions
void CPU::ADC() {

}

void CPU::AND() {

}

void CPU::ASL() {

}

void CPU::BCC() {

}

void CPU::BCS() {

}

void CPU::BEQ() {

}

void CPU::BIT() {

}

void CPU::BMI() {

}

void CPU::BNE() {

}

void CPU::BPL() {

}

void CPU::BRK() {

}

void CPU::BVC() {

}

void CPU::BVS() {

}

void CPU::CLC() {

}

void CPU::CLD() {

}

void CPU::CLI() {

}

void CPU::CLV() {

}

void CPU::CMP() {

}

void CPU::CPX() {

}

void CPU::CPY() {

}

void CPU::DEC() {

}

void CPU::DEX() {

}

void CPU::DEY() {

}

void CPU::EOR() {

}

void CPU::INC() {

}

void CPU::INX() {

}

void CPU::INY() {

}

void CPU::JMP() {

}

void CPU::JSR() {

}

void CPU::LDA() {
    registers.A = bus.data;
}

void CPU::LDX() {

}

void CPU::LDY() {

}

void CPU::LSR() {

}

void CPU::NOP() {

}

void CPU::ORA() {

}

void CPU::PHA() {

}

void CPU::PHP() {

}

void CPU::PLA() {

}

void CPU::PLP() {

}

void CPU::ROL() {

}

void CPU::ROR() {

}

void CPU::RTI() {

}

void CPU::RTS() {

}

void CPU::SBC() {

}

void CPU::SEC() {

}

void CPU::SED() {

}

void CPU::SEI() {

}

void CPU::STA() {

}

void CPU::STX() {

}

void CPU::STY() {

}

void CPU::TAX() {

}

void CPU::TAY() {

}

void CPU::TSX() {

}

void CPU::TXA() {

}

void CPU::TXS() {

}

void CPU::TYA() {

}

