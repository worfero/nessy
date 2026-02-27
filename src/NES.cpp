#include "../include/NES.h"

NES::NES() : bus(), 
             cpu(bus),
             sram()
{
    bus.connectSRAM(&sram);
}

void NES::clock(){
    if(cycleCount % 3 == 0){
        cpu.clock();
    }

    cycleCount++;
}

void NES::reset(){
    cpu.reset();
    cycleCount = 0;
}

void NES::setCpuFlag(CPU::StatusFlag flag, bool value){
    cpu.setFlag(flag, value);
}

void NES::printCpuRegisters(){
    cpu.printRegisters();
}