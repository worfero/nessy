#ifndef RAM_H
#define RAM_H

#include <cstdint>
#include <array>

#define RAM_SIZE 0x800

class RAM{
public:
    RAM();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);

    void printMemoryMap(uint16_t startAddr, uint16_t rows);

private:
    std::array<uint8_t, RAM_SIZE> memory{};
};

#endif