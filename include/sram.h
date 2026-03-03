#ifndef SRAM_H
#define SRAM_H

#include <cstdint>
#include <array>

#define RAM_SIZE 2048

class SRAM{
public:
    SRAM();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);

    void printMemoryMap(uint16_t startAddr, uint16_t rows);

private:
    std::array<uint8_t, RAM_SIZE> memory{};
};

#endif