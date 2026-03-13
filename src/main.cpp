#include "../include/NES.h"

int main(){
    NES nes;
    nes.powerOn();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();
    
    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printMemoryMap(0x0050, 5);
    nes.printMemoryMap(0x0030, 1);
    nes.printMemoryMap(0x0090, 1);
    nes.printMemoryMap(0x9000, 1);
    nes.printMemoryMap(0x9060, 1);
    nes.printCycleCounts();
    nes.printCpuRegisters();

    return 0;
}