#ifndef PPU_H
#define PPU_H

#include "../include/ram.h"
#include "../include/cartridge.h"
#include "../include/cpu.h"

#include <cstdint>

#define CHR_ROM_START_ADDR      0x0000
#define NAMETABLE_START_ADDR    0x2000
#define PALETTE_START_ADDR      0x3F00
#define MIRROR_START_ADDR       0x4000

#define FINE_Y                  ((v_reg >> 12) & 0x07)

class Cartridge;
class CPU;

class PPU{
    public:
        PPU();

        enum StatusFlag : uint8_t{
            SPRITE_OVERFLOW    = 1 << 5,
            SPRITE_ZERO_HIT    = 1 << 6,
            VBLANK             = 1 << 7
        };

        void connectCartridge(Cartridge *selec_cartridge);
        void connectCPU(CPU *selec_cpu);

        void clock();

        void setFlag(StatusFlag flag, bool value);
        bool getFlag(StatusFlag flag) const;

        uint8_t readReg(uint16_t addr);
        void writeReg(uint16_t addr, uint8_t data);

        bool getW() const;
        uint16_t getT() const;
        uint16_t getV() const;
        uint8_t getX() const;

        uint16_t getCycle() const;
        int16_t getScanline() const;

        void printRegisters();
        void printState();
        void printCurrentTile();

    private:
        RAM vram;
        Cartridge *cartridge;
        CPU *cpu;

        std::array<uint8_t, 32> paletteRam{};

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

        // state registers
        bool w_latch;
        uint16_t t_reg;
        uint16_t v_reg;
        uint8_t x_reg;
        uint8_t readBuffer;

        // tile fetch registers
        uint8_t nextTileID;
        uint8_t nextAttribute;
        uint8_t lowBitPattern;
        uint8_t hiBitPattern;

        // shift registers
        uint16_t bgPatternLowBits;
        uint16_t bgPatternHiBits;
        uint16_t bgAttributeLowBits;
        uint16_t bgAttributeHiBits;
        
        // graphic generation registers
        uint16_t paletteAddress;

        // cycle variables
        uint16_t cycle;
        int16_t scanline;

        // operational functions
        void updateVerticalBits();
        void updateHorizontalBits();

        Registers registers;

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

        void incrementFetchAddr();
        void incrementCoarseX();
        void incrementFineY();
        void incrementCoarseY();
};

#endif