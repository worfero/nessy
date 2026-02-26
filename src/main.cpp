#include "../include/cpu.h"
#include "../include/sram.h"
#include "../include/bus.h"

int main(){
    CPU cpu;
    Bus bus;
    SRAM sram;

    cpu.connectBus(&bus);
    bus.connectSRAM(&sram);

    cpu.reset();
    cpu.printRegisters();

    cpu.setFlag(CPU::CARRY, true);
    cpu.printRegisters();

    bus.write(0x000A, 8);

    sram.printMemoryMap();

    return 0;
}