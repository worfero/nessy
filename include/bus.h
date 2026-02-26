#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>

class SRAM;

class Bus {
    public:
        Bus();

        void connectSRAM(SRAM *selec_sram);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

    private:
        SRAM *sram = nullptr;
};

#endif