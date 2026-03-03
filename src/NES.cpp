#include "../include/NES.h"

#include <cstdio>

NES::NES() : bus(), 
             cpu(bus),
             sram(),
             cartridge()
{
    bus.connectSRAM(&sram);
    bus.connectCartridge(&cartridge);

    loadTestProgram();
    cpu.reset();

    isRunning = false;
}

void NES::run(){
    isRunning = true;
    while(isRunning){
        clock();
    }
}

void NES::stop(){
    isRunning = false;
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

void NES::loadTestProgram(){
    // reset vector
    bus.write(CARTRIDGE_START_ADDR + 0x7FFC, 0x00);
    bus.write(CARTRIDGE_START_ADDR + 0x7FFD, 0x80);

    // LDA #$42 | 0xA9 0x42
    bus.write(CARTRIDGE_START_ADDR, 0xA9);
    bus.write(CARTRIDGE_START_ADDR + 0x0001, 0x42);
}

void NES::setCpuFlag(CPU::StatusFlag flag, bool value){
    cpu.setFlag(flag, value);
}

void NES::printCpuRegisters(){
    cpu.printRegisters();
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