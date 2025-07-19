#include <cstdint>
#include <array>
#include <string>
#include <map>
#include <vector>

#include "types.hpp"


#ifndef CPU_H
#define CPU_H

class CPU {
    public:
        std::vector<CPUTypes::InstructionDef> instructionTable;

        CPUTypes::Registers registers;
        NESTypes::Bus &bus;

        uint8_t cyclesRemaining;
        uint8_t resetVector[2];
        std::array<uint16_t, 3> instruction;


        CPU(NESTypes::Bus &b);

        void initInstructionTable();

        uint8_t getFlagValue(CPUTypes::Flag f);
        void setStatusFlagValue(CPUTypes::Flag f, bool set);
        void setStatusFlags(uint8_t flags, bool set);

        void executeInstruction();
        void flattenInstructionArgument();
        void incrementPC();
        void printRegisters();

        void aBusAddXReg(CPUTypes::AddressingMode mode);
        void aBusAddYReg(CPUTypes::AddressingMode mode);
        void aBusLoadInstructionArgument();
        void aBusLoadPC();
        void aBusStoreResetVector();

        void dBusLoadInstructionArgument();
        void dBusLoadRegister(CPUTypes::RegisterName r);
        void dBusStoreInstruction(int i);

        void rwBusSetSignal();


        // Instructions 
        void ADC();
        void AND();
        void ASL();
        void BCC();
        void BCS();

        void BEQ();
        void BIT();
        void BMI();
        void BNE();
        void BPL();
        
        void BRK();
        void BVC();
        void BVS();
        void CLC();
        void CLD();
        
        void CLI();
        void CLV();
        void CMP();
        void CPX();
        void CPY();
        
        void DEC();
        void DEX();
        void DEY();
        void EOR();
        void INC();
        
        void INX();
        void INY();
        void JMP();
        void JSR();
        void LDA();
        
        void LDX();
        void LDY();
        void LSR();
        void NOP();
        void ORA();
        
        void PHA();
        void PHP();
        void PLA();
        void PLP();
        void ROL();
        
        void ROR();
        void RTI();
        void RTS();
        void SBC();
        void SEC();
        
        void SED();
        void SEI();
        void STA();
        void STX();
        void STY();
        
        void TAX();
        void TAY();
        void TSX();
        void TXA();
        void TXS();
        
        void TYA();
};


#endif
