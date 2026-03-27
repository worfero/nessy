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
    nes.printCpuFlags();

    return 0;
}