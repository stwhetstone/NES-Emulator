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

    // lambda because std::bindis slow and to do void(CPU::*fnc), instructiondefs would have to be in CPU.hpp
    auto adcl = [this](){this->ADC();};
    instructionTable[0x69] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, adcl};
    instructionTable[0x65] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, adcl};
    instructionTable[0x75] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, adcl};
    instructionTable[0x6d] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, adcl};
    instructionTable[0x7d] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, adcl};
    instructionTable[0x79] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, adcl};
    instructionTable[0x61] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, adcl};
    instructionTable[0x71] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, adcl};

    auto andl = [this](){this->AND();};
    instructionTable[0x29] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, andl};
    instructionTable[0x25] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, andl};
    instructionTable[0x35] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, andl};
    instructionTable[0x2d] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, andl};
    instructionTable[0x3d] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, andl};
    instructionTable[0x39] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, andl};
    instructionTable[0x21] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, andl};
    instructionTable[0x31] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, andl};

    auto asll = [this](){this->ASL();};
    instructionTable[0x0a] = {1, 2, 1, CPUTypes::AddressingMode::Accumulator, asll};
    instructionTable[0x06] = {2, 5, 1, CPUTypes::AddressingMode::ZeroPage, asll};
    instructionTable[0x16] = {2, 6, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, asll};
    instructionTable[0x0e] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, asll};
    instructionTable[0x1e] = {3, 7, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, asll};

    auto bccl = [this](){this->BCC();};
    instructionTable[0x90] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bccl};

    auto bcsl = [this](){this->BCS();};
    instructionTable[0xB0] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bcsl};

    auto beql = [this](){this->BEQ();};
    instructionTable[0xF0] = {2, 2, 1, CPUTypes::AddressingMode::Relative, beql};

    auto bitl = [this](){this->BIT();};
    instructionTable[0x24] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, bitl};
    instructionTable[0x2c] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, bitl};

    auto bmil = [this](){this->BMI();};
    instructionTable[0x30] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bmil};

    auto bnel = [this](){this->BNE();};
    instructionTable[0xd0] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bnel};

    auto bpll = [this](){this->BPL();};
    instructionTable[0x10] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bpll};

    auto brkl = [this](){this->BRK();};
    instructionTable[0x00] = {1, 7, 1, CPUTypes::AddressingMode::Implied, brkl};

    auto bvcl = [this](){this->BVC();};
    instructionTable[0x50] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bvcl};

    auto bvsl = [this](){this->BVS();};
    instructionTable[0x70] = {2, 2, 1, CPUTypes::AddressingMode::Relative, bvsl};

    auto clcl = [this](){this->CLC();};
    instructionTable[0x18] = {1, 2, 1, CPUTypes::AddressingMode::Implied, clcl};

    auto cldl = [this](){this->CLD();};
    instructionTable[0xd8] = {1, 2, 1, CPUTypes::AddressingMode::Implied, cldl};

    auto clil = [this](){this->CLI();};
    instructionTable[0x58] = {1, 2, 1, CPUTypes::AddressingMode::Implied, clil};

    auto clvl = [this](){this->CLV();};
    instructionTable[0xb8] = {1, 2, 1, CPUTypes::AddressingMode::Implied, clvl};

    auto cmpl = [this](){this->CMP();};
    instructionTable[0xc9] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, cmpl};
    instructionTable[0xc5] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, cmpl};
    instructionTable[0xd5] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, cmpl};
    instructionTable[0xcd] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, cmpl};
    instructionTable[0xdd] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, cmpl};
    instructionTable[0xd9] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, cmpl};
    instructionTable[0xc1] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, cmpl};
    instructionTable[0xd1] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, cmpl};

    auto cpxl = [this](){this->CPX();};
    instructionTable[0xe0] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, cpxl};
    instructionTable[0xe4] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, cpxl};
    instructionTable[0xec] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, cpxl};

    auto cpyl = [this](){this->CPY();};
    instructionTable[0xc0] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, cpyl};
    instructionTable[0xc4] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, cpyl};
    instructionTable[0xcc] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, cpyl};

    auto decl = [this](){this->DEC();};
    instructionTable[0xc6] = {2, 5, 1, CPUTypes::AddressingMode::ZeroPage, decl};
    instructionTable[0xd6] = {2, 6, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, decl};
    instructionTable[0xce] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, decl};
    instructionTable[0xde] = {3, 7, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, decl};

    auto dexl = [this](){this->DEX();};
    instructionTable[0xca] = {1, 2, 1, CPUTypes::AddressingMode::Implied, dexl};

    auto deyl = [this](){this->DEY();};
    instructionTable[0x88] = {1, 2, 1, CPUTypes::AddressingMode::Implied, deyl};

    auto eorl = [this](){this->EOR();};
    instructionTable[0x49] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, eorl};
    instructionTable[0x45] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, eorl};
    instructionTable[0x55] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, eorl};
    instructionTable[0x4d] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, eorl};
    instructionTable[0x5d] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, eorl};
    instructionTable[0x59] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, eorl};
    instructionTable[0x41] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, eorl};
    instructionTable[0x51] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, eorl};

    auto incl = [this](){this->INC();};
    instructionTable[0xe6] = {2, 5, 1, CPUTypes::AddressingMode::ZeroPage, incl};
    instructionTable[0xf6] = {2, 6, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, incl};
    instructionTable[0xee] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, incl};
    instructionTable[0xfe] = {3, 7, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, incl};

    auto inxl = [this](){this->INX();};
    instructionTable[0xe8] = {1, 2, 1, CPUTypes::AddressingMode::Implied, inxl};

    auto inyl = [this](){this->INY();};
    instructionTable[0xc8] = {1, 2, 1, CPUTypes::AddressingMode::Implied, inyl};

    auto jmpl = [this](){this->JMP();};
    instructionTable[0x4c] = {3, 3, 1, CPUTypes::AddressingMode::Absolute, jmpl};
    instructionTable[0x6c] = {3, 5, 1, CPUTypes::AddressingMode::Indirect, jmpl};

    auto jsrl = [this](){this->JSR();};
    instructionTable[0x20] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, jsrl};

    auto ldal = [this](){this->LDA();};
    instructionTable[0xa9] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, ldal};
    instructionTable[0xa5] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, ldal};
    instructionTable[0xb5] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, ldal};
    instructionTable[0xad] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, ldal};
    instructionTable[0xbd] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, ldal};
    instructionTable[0xb9] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, ldal};
    instructionTable[0xa1] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, ldal};
    instructionTable[0xb1] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, ldal};

    auto ldxl = [this](){this->LDX();};
    instructionTable[0xa2] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, ldxl};
    instructionTable[0xa6] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, ldxl};
    instructionTable[0xb6] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedY, ldxl};
    instructionTable[0xae] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, ldxl};
    instructionTable[0xbe] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, ldxl};

    auto ldyl = [this](){this->LDY();};
    instructionTable[0xa0] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, ldyl};
    instructionTable[0xa4] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, ldyl};
    instructionTable[0xb4] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, ldyl};
    instructionTable[0xac] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, ldyl};
    instructionTable[0xbc] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, ldyl};

    auto lsrl = [this](){this->LSR();};
    instructionTable[0x4a] = {1, 2, 1, CPUTypes::AddressingMode::Accumulator, lsrl};
    instructionTable[0x46] = {2, 5, 1, CPUTypes::AddressingMode::ZeroPage, lsrl};
    instructionTable[0x56] = {2, 6, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, lsrl};
    instructionTable[0x4e] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, lsrl};
    instructionTable[0x5e] = {3, 7, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, lsrl};

    auto nopl = [this](){this->NOP();};
    instructionTable[0xea] = {1, 2, 1, CPUTypes::AddressingMode::Implied, nopl};

    auto oral = [this](){this->ORA();};
    instructionTable[0x09] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, oral};
    instructionTable[0x05] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, oral};
    instructionTable[0x15] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, oral};
    instructionTable[0x0d] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, oral};
    instructionTable[0x1d] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, oral};
    instructionTable[0x19] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, oral};
    instructionTable[0x01] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, oral};
    instructionTable[0x11] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, oral};

    auto phal = [this](){this->PHA();};
    instructionTable[0x48] = {1, 3, 1, CPUTypes::AddressingMode::Implied, phal};

    auto phpl = [this](){this->PHP();};
    instructionTable[0x08] = {1, 3, 1, CPUTypes::AddressingMode::Implied, phpl};

    auto plal = [this](){this->PLA();};
    instructionTable[0x68] = {1, 4, 1, CPUTypes::AddressingMode::Implied, plal};

    auto plpl = [this](){this->PLP();};
    instructionTable[0x28] = {1, 4, 1, CPUTypes::AddressingMode::Implied, plpl};

    auto roll = [this](){this->ROL();};
    instructionTable[0x2a] = {1, 2, 1, CPUTypes::AddressingMode::Accumulator, roll};
    instructionTable[0x26] = {2, 5, 1, CPUTypes::AddressingMode::ZeroPage, roll};
    instructionTable[0x36] = {2, 6, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, roll};
    instructionTable[0x2e] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, roll};
    instructionTable[0x3e] = {3, 7, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, roll};

    auto rorl = [this](){this->ROR();};
    instructionTable[0x6a] = {1, 2, 1, CPUTypes::AddressingMode::Accumulator, rorl};
    instructionTable[0x66] = {2, 5, 1, CPUTypes::AddressingMode::ZeroPage, rorl};
    instructionTable[0x76] = {2, 6, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, rorl};
    instructionTable[0x6e] = {3, 6, 1, CPUTypes::AddressingMode::Absolute, rorl};
    instructionTable[0x7e] = {3, 7, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, rorl};

    auto rtil = [this](){this->RTI();};
    instructionTable[0x40] = {1, 6, 1, CPUTypes::AddressingMode::Implied, rtil};

    auto rtsl = [this](){this->RTS();};
    instructionTable[0x60] = {1, 6, 1, CPUTypes::AddressingMode::Implied, rtsl};

    auto sbcl = [this](){this->SBC();};
    instructionTable[0xe9] = {2, 2, 1, CPUTypes::AddressingMode::Immediate, sbcl};
    instructionTable[0xe5] = {2, 3, 1, CPUTypes::AddressingMode::ZeroPage, sbcl};
    instructionTable[0xf5] = {2, 4, 1, CPUTypes::AddressingMode::ZeroPageIndexedX, sbcl};
    instructionTable[0xed] = {3, 4, 1, CPUTypes::AddressingMode::Absolute, sbcl};
    instructionTable[0xfd] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedX, sbcl};
    instructionTable[0xf9] = {3, 4, 1, CPUTypes::AddressingMode::AbsoluteIndexedY, sbcl};
    instructionTable[0xe1] = {2, 6, 1, CPUTypes::AddressingMode::IndexedIndirectX, sbcl};
    instructionTable[0xf1] = {2, 5, 1, CPUTypes::AddressingMode::IndirectIndexedY, sbcl};

    auto secl = [this](){this->SEC();};
    instructionTable[0x38] = {1, 2, 1, CPUTypes::AddressingMode::Implied, secl};

    auto sedl = [this](){this->SED();};
    instructionTable[0xf8] = {1, 2, 1, CPUTypes::AddressingMode::Implied, sedl};

    auto seil = [this](){this->SEI();};
    instructionTable[0x78] = {1, 2, 1, CPUTypes::AddressingMode::Implied, seil};

    auto stal = [this](){this->STA();};
    instructionTable[0x85] = {2, 3, 0, CPUTypes::AddressingMode::ZeroPage, stal};
    instructionTable[0x95] = {2, 4, 0, CPUTypes::AddressingMode::ZeroPageIndexedX, stal};
    instructionTable[0x8d] = {3, 4, 0, CPUTypes::AddressingMode::Absolute, stal};
    instructionTable[0x9d] = {3, 5, 0, CPUTypes::AddressingMode::AbsoluteIndexedX, stal};
    instructionTable[0x99] = {3, 5, 0, CPUTypes::AddressingMode::AbsoluteIndexedY, stal};
    instructionTable[0x81] = {2, 6, 0, CPUTypes::AddressingMode::IndexedIndirectX, stal};
    instructionTable[0x91] = {2, 6, 0, CPUTypes::AddressingMode::IndirectIndexedY, stal};

    auto stxl = [this](){this->STX();};
    instructionTable[0x86] = {2, 3, 0, CPUTypes::AddressingMode::ZeroPage, stxl};
    instructionTable[0x96] = {2, 4, 0, CPUTypes::AddressingMode::ZeroPageIndexedY, stxl};
    instructionTable[0x8e] = {3, 4, 0, CPUTypes::AddressingMode::Absolute, stxl};

    auto styl = [this](){this->STY();};
    instructionTable[0x84] = {2, 3, 0, CPUTypes::AddressingMode::ZeroPage, styl};
    instructionTable[0x94] = {2, 4, 0, CPUTypes::AddressingMode::ZeroPageIndexedX, styl};
    instructionTable[0x8c] = {3, 4, 0, CPUTypes::AddressingMode::Absolute, styl};

    auto taxl = [this](){this->TAX();};
    instructionTable[0xaa] = {1, 2, 1, CPUTypes::AddressingMode::Implied, taxl};

    auto tayl = [this](){this->TAY();};
    instructionTable[0xa8] = {1, 2, 1, CPUTypes::AddressingMode::Implied, tayl};

    auto tsxl = [this](){this->TSX();};
    instructionTable[0xba] = {1, 2, 1, CPUTypes::AddressingMode::Implied, tsxl};

    auto txal = [this](){this->TXA();};
    instructionTable[0x8a] = {1, 2, 1, CPUTypes::AddressingMode::Implied, txal};

    auto txsl = [this](){this->TXS();};
    instructionTable[0x9a] = {1, 2, 1, CPUTypes::AddressingMode::Implied, txsl};

    auto tyal = [this](){this->TYA();};
    instructionTable[0x98] = {1, 2, 1, CPUTypes::AddressingMode::Implied, tyal};
}


uint8_t CPU::getFlagValue(CPUTypes::Flag f) {
    return (registers.status & (1 << f)) >> f;
}


void CPU::setStatusFlagValue(CPUTypes::Flag f, bool set) {
    if(f == CPUTypes::Flag::One) {
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
    std::cout << "\n       NV1B DIZC" << '\n';
    
    std::cout << "PC " << std::hex << (unsigned)registers.PC << '\n';
    std::cout << std::endl;
}


void CPU::aBusAddXReg(CPUTypes::AddressingMode mode) {
    if(mode == CPUTypes::ZeroPageIndexedX) {
        uint16_t temp = bus.address;
        bus.address = (temp + registers.X) % 256;
    } else if(mode == CPUTypes::AbsoluteIndexedX) {
        bus.address += registers.X; 
    }
}

void CPU::aBusAddYReg(CPUTypes::AddressingMode mode) {
    if(mode == CPUTypes::ZeroPageIndexedY) {
        uint16_t temp = bus.address;
        bus.address = (temp + registers.Y) % 256;
    } else if(mode == CPUTypes::AbsoluteIndexedY) {
        bus.address += registers.Y; 
    }
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
    uint8_t tmpA = registers.A;
    registers.A += bus.data + (registers.status & 1);

    setStatusFlagValue(CPUTypes::Flag::C, registers.A > 0xff);
    setStatusFlagValue(CPUTypes::Flag::Z, registers.A == 0);
    setStatusFlagValue(CPUTypes::Flag::V, (registers.A ^ tmpA) & (registers.A ^ bus.data) & 0x80);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.A >> 7) == 1);
}

void CPU::AND() {
    registers.A &= bus.data;

    setStatusFlagValue(CPUTypes::Flag::N, (registers.A >> 7) == 1);
    setStatusFlagValue(CPUTypes::Flag::Z, registers.A == 0);
}

void CPU::ASL() {
    if(instructionTable[instruction[0]].size == 1) {
        setStatusFlagValue(CPUTypes::Flag::C, (registers.A >> 7) == 1);

        registers.A <<= 1;

        setStatusFlagValue(CPUTypes::Flag::Z, registers.A == 0);
        setStatusFlagValue(CPUTypes::Flag::N, (registers.A >> 7) == 1);
    } else {
        setStatusFlagValue(CPUTypes::Flag::C, (bus.data >> 7) == 1);

        bus.data <<= 1;
        bus.rwSignal = 0;

        setStatusFlagValue(CPUTypes::Flag::Z, bus.data == 0);
        setStatusFlagValue(CPUTypes::Flag::N, (bus.data >> 7) == 1);
    }
}

void CPU::BCC() {
    if(getFlagValue(CPUTypes::Flag::C) == 0) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BCS() {
    if(getFlagValue(CPUTypes::Flag::C) == 1) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BEQ() {
    if(getFlagValue(CPUTypes::Flag::Z) == 1) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BIT() {
    setStatusFlagValue(CPUTypes::Flag::Z, (registers.A & bus.data) == 0);
    setStatusFlagValue(CPUTypes::Flag::V, ((bus.data >> 6) & 1) == 1);
    setStatusFlagValue(CPUTypes::Flag::N, (bus.data >> 7) == 1);
}

void CPU::BMI() {
    if(getFlagValue(CPUTypes::Flag::N) == 1) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BNE() {
    if(getFlagValue(CPUTypes::Flag::Z) == 0) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BPL() {
    if(getFlagValue(CPUTypes::Flag::N) == 0) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BRK() {

}

void CPU::BVC() {
    if(getFlagValue(CPUTypes::Flag::V) == 0) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::BVS() {
    if(getFlagValue(CPUTypes::Flag::V) == 1) {
        int8_t signedArgument = bus.data;
        registers.PC += 2 + signedArgument;
    }
}

void CPU::CLC() {
    setStatusFlagValue(CPUTypes::Flag::C, false);
}

void CPU::CLD() {
    setStatusFlagValue(CPUTypes::Flag::D, false);
}

void CPU::CLI() {

}

void CPU::CLV() {
    setStatusFlagValue(CPUTypes::Flag::V, false);
}

void CPU::CMP() {
    uint8_t cmpValue = registers.A - bus.data;

    setStatusFlagValue(CPUTypes::Flag::C, cmpValue >= 0);
    setStatusFlagValue(CPUTypes::Flag::Z, cmpValue == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (cmpValue >> 7) == 1);
}

void CPU::CPX() {
    uint8_t cmpValue = registers.X - bus.data;

    setStatusFlagValue(CPUTypes::Flag::C, cmpValue >= 0);
    setStatusFlagValue(CPUTypes::Flag::Z, cmpValue == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (cmpValue >> 7) == 1);
}

void CPU::CPY() {
    uint8_t cmpValue = registers.Y - bus.data;

    setStatusFlagValue(CPUTypes::Flag::C, cmpValue >= 0);
    setStatusFlagValue(CPUTypes::Flag::Z, cmpValue == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (cmpValue >> 7) == 1);
}

void CPU::DEC() {
    bus.data -= 1;

    setStatusFlagValue(CPUTypes::Flag::Z, bus.data == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (bus.data >> 7) == 1);

    bus.rwSignal = 0;
}

void CPU::DEX() {
    registers.X -= 1;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.X == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.X >> 7) == 1);
}

void CPU::DEY() {
    registers.Y -= 1;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.Y == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.Y >> 7) == 1);
}

void CPU::EOR() {
    registers.A ^= bus.data;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.A == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.A >> 7) == 1);
}

void CPU::INC() {
    bus.data += 1;

    setStatusFlagValue(CPUTypes::Flag::Z, bus.data == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (bus.data >> 7) == 1);

    bus.rwSignal = 0;
}

void CPU::INX() {
    registers.X += 1;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.X == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.X >> 7) == 1);
}

void CPU::INY() {
    registers.Y += 1;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.Y == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.Y >> 7) == 1);
}

void CPU::JMP() {

}

void CPU::JSR() {

}

void CPU::LDA() {
    registers.A = bus.data;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.A == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.A >> 7) == 1);
}

void CPU::LDX() {
    registers.X = bus.data;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.X == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.X >> 7) == 1);
}

void CPU::LDY() {
    registers.Y = bus.data;

    setStatusFlagValue(CPUTypes::Flag::Z, registers.Y == 0);
    setStatusFlagValue(CPUTypes::Flag::N, (registers.Y >> 7) == 1);
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
    setStatusFlagValue(CPUTypes::Flag::C, true);
}

void CPU::SED() {

}

void CPU::SEI() {

}

void CPU::STA() {
    dBusLoadRegister(CPUTypes::RegisterName::A);
}

void CPU::STX() {
    dBusLoadRegister(CPUTypes::RegisterName::X);
}

void CPU::STY() {
    dBusLoadRegister(CPUTypes::RegisterName::Y);
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

