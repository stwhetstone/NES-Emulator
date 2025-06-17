#include <cstdint>
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
        void setFlagValue(CPUTypes::Flag f, uint8_t byte);

        void executeInstruction();
        void flattenInstructionArgument();
        void incrementPC();
        void printRegisters();

        void aBusLoadInstructionArgument();
        void aBusLoadPC();
        void aBusStoreResetVector();

        void dBusLoadRegister(CPUTypes::RegisterName r);
        void dBusStoreInstruction(int i);

        void rwBusSetSignal();


        // Instructions 
        // Access
        void LDA();
        void LDX();
        void LDY();
        void STA();
        void STX();
        void STY();

        // Transfer
        void TAX();
        void TAY();
        void TXA();
        void TYA();

        // Arithmetic
        void ADC();
        void DEC();
        void DEX();
        void DEY();
        void INC();
        void INX();
        void INY();
        void SBC();

        // Shift
        void ASL();
        void LSR();
        void ROL();
        void ROR();

        // Bitwise
        void AND();
        void BIT();
        void EOR();
        void ORA();

        // Compare
        void CMP();
        void CPX();
        void CPY();

        // Branch
        void BCC();
        void BCS();
        void BEQ();
        void BMI();
        void BNE();
        void BPL();
        void BVC();
        void BVS();

        // Jump
        void BRK();
        void JMP();
        void JSR();
        void RTI();
        void RTS();

        // Stack
        void PHA();
        void PHP();
        void PLA();
        void PLP();
        void TSX();
        void TXS();

        // Flags
        void CLC();
        void CLD();
        void CLI();
        void CLV();
        void SEC();
        void SED();
        void SEI();

        // Other
        void NOP();
};


#endif
