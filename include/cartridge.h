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

    uint8_t readPgr(uint16_t address);

    uint8_t readChr(uint16_t address);
    void writeChr(uint16_t address, uint8_t data);

private:
    std::vector<uint8_t> pgrRom{};
    std::vector<uint8_t> chrRom{};

    bool allowChrWrite = false;
};

#endif