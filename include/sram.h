#ifndef SRAM_H
#define SRAM_H

#include <cstdint>
#include <array>

class SRAM{
public:
    SRAM();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);

    void printMemoryMap();

private:
    std::array<uint8_t, 2048> memory{};
};

#endif