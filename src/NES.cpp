#include "../include/NES.h"

#include <cstdio>

NES::NES() : bus(), 
             cpu(bus),
             sram(),
             cartridge()
{
    bus.connectSRAM(&sram);

    isRunning = false;
}

void NES::powerOn(){
    cartridge.loadRomFile("test.nes");
    bus.connectCartridge(&cartridge);
    cpu.reset();
}

void NES::run(){
    isRunning = true;
    while(isRunning){
        clock();
    }
}

void NES::stepCpu(){
    // make sure the current instruction gets finished (if there is one to begin with)
    while(cpu.getInstructionCycleCounter() > 0){
        clock();
    }

    // clock until it's a CPU clock time
    do{
        clock();
    }
    while(!cpuClock);

    // fetch next instruction and wait until it's finished
    while(cpu.getInstructionCycleCounter() > 0){
        clock();
    };
}

void NES::runCycles(int16_t cycles){
    isRunning = true;
    while(isRunning){
        if(cycles == 0){
            stop();
        }
        clock();
        cycles--;
    }
}

void NES::stop(){
    isRunning = false;
}

void NES::clock(){
    if(cycleCount % 3 == 0){
        cpu.clock();
        cpuClock = true;
    }
    else{
        cpuClock = false;
    }

    cycleCount++;
}

void NES::reset(){
    cpu.reset();
    cycleCount = 0;
}

void NES::printCycleCounts(){
    std::printf("---------Cycles--------- \nSystem cycles: %lu \nCPU cycles: %lu\nCPU Steps: %lu\n\n", cycleCount, getCpuCycleCount(), getCpuSteps());
}

void NES::setCpuFlag(CPU::StatusFlag flag, bool value){
    cpu.setFlag(flag, value);
}

bool NES::getCpuFlag(CPU::StatusFlag flag) const{
    return cpu.getFlag(flag);
}

uint64_t NES::getCpuCycleCount() const{
    return cpu.getCycleCount();
}

uint64_t NES::getCpuSteps() const{
    return cpu.getStepCount();
}

void NES::printCpuRegisters(){
    cpu.printRegisters();
}

void NES::printCpuFlags(){
    cpu.printFlags();
}

void NES::printMemoryMap(uint16_t startAddr, uint16_t rows){
    const uint16_t bytesPerRow = 8;
    uint16_t baseAddress = startAddr & 0xFFF8; // row allignment

    for(uint16_t row = 0; row < rows; row++){
        if(baseAddress < MEMORY_MAP_SIZE){
            std::printf("0x%04X: ", baseAddress);
            for(uint16_t idx = 0; idx < bytesPerRow; idx++){
                uint16_t readAddress = baseAddress + idx;
                uint8_t value = bus.read(readAddress);
                std::printf("0x%02X ", value);
            }
            std::printf("\n");
        } else{
            break;
        }

        baseAddress += bytesPerRow;
    }
    std::printf("\n");
}