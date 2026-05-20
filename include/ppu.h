#ifndef PPU_H
#define PPU_H

#include "../include/ram.h"
#include "../include/cartridge.h"
#include "../include/cpu.h"

#include <cstdint>
#include <iostream>

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

        bool frameComplete;

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
        uint32_t *getFramebuffer();

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

        // palette lookup table
        static constexpr uint32_t nesPalette[64] = {
            0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4,
            0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00,
            0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08,
            0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000,

            0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE,
            0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00,
            0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32,
            0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000,

            0xFFFFFEFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF,
            0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22,
            0xFFBCBE00, 0xFF88D800, 0xFF5CE430, 0xFF45E082,
            0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000,

            0xFFFFFEFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF,
            0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5,
            0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC,
            0xFFB5EBF2, 0xFFB8B8B8, 0xFF000000, 0xFF000000
        };

        // state registers
        Registers registers;
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
        
        // graphic generation variables
        uint16_t paletteAddress;
        std::array<uint32_t, 256 * 240> framebuffer;

        // cycle variables
        uint16_t cycle;
        int16_t scanline;

        // operational functions
        void updateVerticalBits();
        void updateHorizontalBits();

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

        void incrementFetchAddr();
        void incrementCoarseX();
        void incrementFineY();
        void incrementCoarseY();
};

#endif