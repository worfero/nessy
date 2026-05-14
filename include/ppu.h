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

        struct RGB {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        // palette lookup table
        static constexpr RGB nesPalette[64] = {
            {102,102,102}, {0,42,136},   {20,18,167},  {59,0,164},
            {92,0,126},    {110,0,64},   {108,6,0},    {86,29,0},
            {51,53,0},     {11,72,0},    {0,82,0},     {0,79,8},
            {0,64,77},     {0,0,0},      {0,0,0},      {0,0,0},

            {173,173,173}, {21,95,217},  {66,64,255},  {117,39,254},
            {160,26,204},  {183,30,123}, {181,49,32},  {153,78,0},
            {107,109,0},   {56,135,0},   {12,147,0},   {0,143,50},
            {0,124,141},   {0,0,0},      {0,0,0},      {0,0,0},

            {255,254,255}, {100,176,255},{146,144,255},{198,118,255},
            {243,106,255}, {254,110,204},{254,129,112},{234,158,34},
            {188,190,0},   {136,216,0},  {92,228,48},  {69,224,130},
            {72,205,222},  {79,79,79},   {0,0,0},      {0,0,0},

            {255,254,255}, {192,223,255},{211,210,255},{232,200,255},
            {251,194,255}, {254,196,234},{254,204,197},{247,216,165},
            {228,229,148}, {207,239,150},{189,244,171},{179,243,204},
            {181,235,242}, {184,184,184},{0,0,0},      {0,0,0}
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
        std::array<RGB, 256 * 240> framebuffer;

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