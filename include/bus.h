#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>

#define PPU_START_ADDR              0x2000
#define APU_CONTROLS_START_ADDR     0x4000
#define WRAM_START_ADDR             0x6000
#define CARTRIDGE_START_ADDR        0x8000
#define MEMORY_MAP_SIZE             0xFFFF

class SRAM;
class Cartridge;

class Bus {
    public:
        Bus();

        void connectSRAM(SRAM *selec_sram);
        void connectCartridge(Cartridge *selec_cartridge);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

    private:
        SRAM *sram = nullptr;
        Cartridge *cartridge = nullptr;
};

#endif