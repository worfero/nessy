#include "../include/NES.h"

int main(){
    NES nes;

    nes.printMemoryMap(0x8000, 1);
    nes.printMemoryMap(0xFFFC, 1);

    nes.printCpuRegisters();

    return 0;
}