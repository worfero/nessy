#ifndef PPU_H
#define PPU_H

#include "../include/ram.h"
#include "../include/cartridge.h"

#define NAMETABLE_START_ADDR    0x2000
#define PALETTE_START_ADDR      0x3F00

class Cartridge;

class PPU{
    public:
        PPU();

        void connectCartridge(Cartridge *selec_cartridge);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

    private:
        RAM vram;
        Cartridge *cartridge;
};

#endif