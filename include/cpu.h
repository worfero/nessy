#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include <array>

#define NMI 0xFFFA
#define RST 0xFFFC
#define IRQ 0xFFFE

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
    uint64_t getStepCount() const;

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
        bool isBranch;
    };

    // instruction lookup table
    std::array<Instruction, 256> instructions;

    Registers registers;
    Bus &bus;

    uint8_t instructionCycles = 0;
    uint64_t cycleCount = 0;
    uint64_t stepCount = 0;
    uint8_t opcode = 0;

    // operation helper functions
    uint8_t fetchValue();
    void writebackValue(uint8_t value);
    uint16_t fetchAddr = 0;

    // stack functions
    void push(uint8_t value);
    uint8_t pop();

    // vector read helper
    uint16_t readVector(uint16_t vectorAddr);

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
    uint8_t TSX();
    uint8_t TXS();
    uint8_t INC();
    uint8_t INX();
    uint8_t INY();
    uint8_t DEC();
    uint8_t DEX();
    uint8_t DEY();
    uint8_t CLC();
    uint8_t CLD();
    uint8_t CLI();
    uint8_t CLV();
    uint8_t SEC();
    uint8_t SED();
    uint8_t SEI();
    uint8_t ADC();
    uint8_t SBC();
    uint8_t CMP();
    uint8_t CPX();
    uint8_t CPY();
    uint8_t BEQ();
    uint8_t BNE();
    uint8_t BCC();
    uint8_t BCS();
    uint8_t BMI();
    uint8_t BPL();
    uint8_t BVC();
    uint8_t BVS();
    uint8_t JMP();
    uint8_t JSR();
    uint8_t RTS();
    uint8_t PHA();
    uint8_t PHP();
    uint8_t PLA();
    uint8_t PLP();
    uint8_t AND();
    uint8_t EOR();
    uint8_t ORA();
    uint8_t ASL();
    uint8_t LSR();
    uint8_t ROL();
    uint8_t ROR();
    uint8_t BIT();
    uint8_t BRK();
    uint8_t RTI();
    uint8_t NOP();

    // unofficial instruction set
    uint8_t ALR();
    uint8_t ANC();
    uint8_t ANC2();
    uint8_t ANE();
    uint8_t ARR();
    

    void fillOpcodes();

    uint8_t readBus(uint16_t address);
    void writeBus(uint16_t address, uint8_t data);
};

#endif