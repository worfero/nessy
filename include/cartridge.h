#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

class Cartridge{
public:
    Cartridge();

    bool loadRomFile(const std::string &filename);

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);

    void printMemoryMap(uint16_t startAddr, uint16_t rows);

private:
    std::vector<uint8_t> memory{};
};

#endif