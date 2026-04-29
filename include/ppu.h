#ifndef PPU_H
#define PPU_H

#include "../include/ram.h"
#include "../include/cartridge.h"

#include <cstdint>

#define NAMETABLE_START_ADDR    0x2000
#define PALETTE_START_ADDR      0x3F00

class Cartridge;

class PPU{
    public:
        PPU();

        enum StatusFlag : uint8_t{
            SPRITE_OVERFLOW    = 1 << 5,
            SPRITE_ZERO_HIT    = 1 << 6,
            VBLANK             = 1 << 7
        };

        void connectCartridge(Cartridge *selec_cartridge);

        void setFlag(StatusFlag flag, bool value);
        bool getFlag(StatusFlag flag) const;

        uint8_t readReg(uint16_t addr);
        void writeReg(uint16_t addr, uint8_t data);

    private:
        RAM vram;
        Cartridge *cartridge;

        struct Registers{
            uint8_t PPUCTRL;
            uint8_t PPUMASK;
            uint8_t PPUSTATUS;
            uint8_t OAMADDR;
            uint8_t OAMDATA;
            uint8_t PPUSCROLL;
            uint8_t PPUADDR;
            uint8_t PPUDATA;
            uint8_t OAMDMA;
        };

        bool ppuAddrLatch;
        uint16_t tempFetchAddr;
        uint16_t fetchAddr;

        Registers registers;

        uint8_t readVram(uint16_t address);
        void writeVram(uint16_t address, uint8_t data);

        void incrementFetchAddr();
};

#endif