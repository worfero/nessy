#ifndef NES_H
#define NES_H

#include "../include/cpu.h"
#include "../include/sram.h"
#include "../include/bus.h"
#include "../include/cartridge.h"

class NES {
    public:
        NES();

        void run();
        void stop();

        void clock();
        void reset();

        uint64_t cycleCount = 0;

        void loadTestProgram();

        // CPU public functions
        void setCpuFlag(CPU::StatusFlag flag, bool value);
        void printCpuRegisters();

        // SRAM public functions
        void printMemoryMap(uint16_t startAddr, uint16_t rows);

    private:
        Bus bus;
        CPU cpu;
        SRAM sram;
        Cartridge cartridge;

        bool isRunning;
};

#endif