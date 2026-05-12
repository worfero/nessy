#include "../include/NES.h"

int main(){
    NES nes;
    nes.powerOn();

    for(int i = 0; i < 179025; i++){
        nes.clock();
    }

    for(int i = 0; i < 340*2; i++){
        nes.clock();
        nes.printCycleCounts();
        nes.printPpuState();
        nes.printPpuTileFetch();
    }

    return 0;
}