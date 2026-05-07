#include "../include/NES.h"

int main(){
    NES nes;
    nes.powerOn();

    nes.printCycleCounts();
    nes.printCpuRegisters();

    for(int i = 0; i < 1000; i++){
        //nes.stepCpu();
        nes.clock();
        nes.printCycleCounts();
        nes.printCpuRegisters();
        nes.printPpuState();
    }

    return 0;
}