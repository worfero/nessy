#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include <array>

class Bus;

class CPU{
public:
    enum StatusFlag : uint8_t{
        CARRY     = 1 << 0,
        ZERO      = 1 << 1,
        INTERRUPT = 1 << 2,
        DECIMAL   = 1 << 3,
        BREAK     = 1 << 4,
        UNUSED    = 1 << 5,
        OVERFLOW  = 1 << 6,
        NEGATIVE  = 1 << 7
    };

    CPU(Bus &bus);

    void clock();
    void reset();

    void setFlag(StatusFlag flag, bool value);
    bool getFlag(StatusFlag flag) const;

    uint8_t getInstructionCycleCounter() const;
    uint64_t getCycleCount() const;

    void printRegisters() const;
    void printFlags() const;

private:
    struct Registers{
        uint8_t A;
        uint8_t X;
        uint8_t Y;
        uint8_t SP;
        uint8_t P;
        uint16_t PC;
    };

    struct Instruction{
        std::string mnemonic;
        uint8_t (CPU::*operation)(void);
        uint8_t (CPU::*addrMode)(void);
        uint8_t cycleCount;
    };

    // instruction lookup table
    std::array<Instruction, 256> instructions;

    Registers registers;
    Bus &bus;

    uint8_t instructionCycles = 0;
    uint64_t cycleCount = 0;
    uint8_t opcode = 0;

    uint8_t fetchValue();
    uint8_t fetchedValue = 0;
    uint16_t fetchAddr = 0;

    // addressing modes
    uint8_t IMP();
    uint8_t ACC();
    uint8_t IMM();
    uint8_t ZP();
    uint8_t ZPX();
    uint8_t ZPY();
    uint8_t ABS();
    uint8_t ABX();
    uint8_t ABY();
    uint8_t IND();
    uint8_t IZX();
    uint8_t IZY();
    uint8_t REL();

    // instruction set
    uint8_t XXX();
    uint8_t LDA();
    uint8_t LDX();
    uint8_t LDY();
    uint8_t STA();
    uint8_t STX();
    uint8_t STY();
    uint8_t TAX();
    uint8_t TAY();
    uint8_t TXA();
    uint8_t TYA();
    uint8_t INX();
    uint8_t INY();
    uint8_t DEX();
    uint8_t DEY();
    uint8_t CLC();
    uint8_t SEC();
    uint8_t ADC();
    uint8_t SBC();
    uint8_t CMP();
    uint8_t CPX();
    uint8_t CPY();

    void fillOpcodes();

    uint8_t readBus(uint16_t address);
    void writeBus(uint16_t address, uint8_t data);
};

#endif