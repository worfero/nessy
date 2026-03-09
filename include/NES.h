#ifndef NES_H
#define NES_H

#include "../include/cpu.h"
#include "../include/sram.h"
#include "../include/bus.h"
#include "../include/cartridge.h"

class NES {
    public:
        NES();

        void powerOn();
        void run();
        void runCycles(int16_t cycles);
        void stop();

        void clock();
        void stepCpu();
        void reset();

        uint64_t cycleCount = 0;
        bool cpuClock = false;

        void loadTestProgram();

        void printCycleCounts();

        // CPU public functions
        void setCpuFlag(CPU::StatusFlag flag, bool value);
        bool getCpuFlag(CPU::StatusFlag flag) const;
        uint8_t getCpuCycleCount() const;
        void printCpuRegisters();
        void printCpuFlags();

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