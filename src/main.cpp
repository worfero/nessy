#include "../include/NES.h"

int main(){
    NES nes;

    nes.printCpuRegisters();

    nes.setCpuFlag(CPU::CARRY, true);
    nes.printCpuRegisters();

    return 0;
}