#include "../include/NES.h"

int main(){
    NES nes;
    nes.powerOn();

    nes.printMemoryMap(0x8000, 1);
    nes.printMemoryMap(0xFFFC, 1);

    nes.printCycleCounts();
    nes.printCpuFlags();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuFlags();
    nes.printCpuRegisters();
    
    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuFlags();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuFlags();
    nes.printCpuRegisters();

    return 0;
}