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
    nes.printMemoryMap(0x0010, 1);

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();
    nes.printMemoryMap(0x0010, 1);

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();
    nes.printMemoryMap(0x1000, 1);

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();
    nes.printMemoryMap(0x0020, 1);

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();
    nes.printMemoryMap(0x0020, 1);

    nes.stepCpu();

    nes.printCycleCounts();
    nes.printCpuRegisters();
    nes.printMemoryMap(0x1400, 1); 

    return 0;
}