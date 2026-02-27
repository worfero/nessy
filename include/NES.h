#ifndef NES_H
#define NES_H

#include "../include/cpu.h"
#include "../include/sram.h"
#include "../include/bus.h"

class NES {
    public:
        NES();

        void clock();
        void reset();

        // CPU public functions forwarder
        void setCpuFlag(CPU::StatusFlag flag, bool value);
        void printCpuRegisters();

        uint64_t cycleCount = 0;

    private:
        Bus bus;
        CPU cpu;
        SRAM sram;
};

#endif