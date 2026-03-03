#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>
#include <array>

#define CARTRIDGE_MEM_SIZE 0x7FFF

class Cartridge{
public:
    Cartridge();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);

    void printMemoryMap(uint16_t startAddr, uint16_t rows);

private:
    std::array<uint8_t, CARTRIDGE_MEM_SIZE> memory{};
};

#endif