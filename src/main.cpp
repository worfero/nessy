#include "../include/NES.h"

int main(){
    NES nes;
    nes.powerOn();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    for(int i = 0; i < 8; i++){
        nes.stepCpu();
        nes.printCycleCounts();
        nes.printCpuRegisters();
        nes.printCpuFlags();
    }

    return 0;
}