#ifndef CPU_H
#define CPU_H

#include <cstdint>

class Bus;

struct Registers{
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t SP;
    uint8_t P;
    uint16_t PC;
};

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

    CPU();

    void reset();

    void setFlag(StatusFlag flag, bool value);
    bool getFlag(StatusFlag flag) const;

    void connectBus(Bus *selec_bus);

    void printRegisters() const;

private:
    Registers registers;
    Bus *bus = nullptr;

    uint8_t readBus(uint16_t address);
    void writeBus(uint16_t address, uint8_t data);
};

#endif