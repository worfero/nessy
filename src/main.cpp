#include "../include/NES.h"

int main(){
    NES nes;
    nes.powerOn();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    for(int i = 0; i < 10; i++){
        nes.stepCpu();
        nes.printCycleCounts();
        nes.printCpuRegisters();
        nes.printCpuFlags();
        nes.printMemoryMap(0x10, 1);
        nes.printMemoryMap(0x50, 1);
    }

    return 0;
}