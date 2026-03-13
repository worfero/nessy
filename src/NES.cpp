#include "../include/NES.h"

#include <cstdio>

NES::NES() : bus(), 
             cpu(bus),
             sram(),
             cartridge()
{
    bus.connectSRAM(&sram);
    bus.connectCartridge(&cartridge);

    isRunning = false;
}

void NES::powerOn(){
    loadTestProgram();
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

void NES::loadTestProgram(){
    // reset vector
    bus.write(CARTRIDGE_START_ADDR + 0x7FFC, 0x00);
    bus.write(CARTRIDGE_START_ADDR + 0x7FFD, 0x80);

    // LDA #$60 | 0xA9 0x60
    bus.write(CARTRIDGE_START_ADDR + 0x0000, 0xA9);
    bus.write(CARTRIDGE_START_ADDR + 0x0001, 0x60);
    // STA $50 | 0x85 0x50
    bus.write(CARTRIDGE_START_ADDR + 0x0002, 0x85);
    bus.write(CARTRIDGE_START_ADDR + 0x0003, 0x50);
    // LDX $50 | 0xA6 0x50
    bus.write(CARTRIDGE_START_ADDR + 0x0004, 0xA6);
    bus.write(CARTRIDGE_START_ADDR + 0x0005, 0x50);
    // STA $10, X | 0x95 0x10
    bus.write(CARTRIDGE_START_ADDR + 0x0006, 0x95);
    bus.write(CARTRIDGE_START_ADDR + 0x0007, 0x10);
    // LDA #$90 | 0xA9 0x90
    bus.write(CARTRIDGE_START_ADDR + 0x0008, 0xA9);
    bus.write(CARTRIDGE_START_ADDR + 0x0009, 0x90);
    // STA $9000 | 0x8D 0x00 0x90
    bus.write(CARTRIDGE_START_ADDR + 0x000A, 0x8D);
    bus.write(CARTRIDGE_START_ADDR + 0x000B, 0x00);
    bus.write(CARTRIDGE_START_ADDR + 0x000C, 0x90);
    // STA $9000, X | 0x9D 0x00 0x90
    bus.write(CARTRIDGE_START_ADDR + 0x000D, 0x9D);
    bus.write(CARTRIDGE_START_ADDR + 0x000E, 0x00);
    bus.write(CARTRIDGE_START_ADDR + 0x000F, 0x90);
    // LDY #$03 | 0xA0 0x03
    bus.write(CARTRIDGE_START_ADDR + 0x0010, 0xA0);
    bus.write(CARTRIDGE_START_ADDR + 0x0011, 0x03);
    // STA $9000, Y | 0x99 0x00 0x90
    bus.write(CARTRIDGE_START_ADDR + 0x0012, 0x99);
    bus.write(CARTRIDGE_START_ADDR + 0x0013, 0x00);
    bus.write(CARTRIDGE_START_ADDR + 0x0014, 0x90);
    // STA $0061 | 0x8D 0x60 0x00
    bus.write(CARTRIDGE_START_ADDR + 0x0015, 0x8D);
    bus.write(CARTRIDGE_START_ADDR + 0x0016, 0x61);
    bus.write(CARTRIDGE_START_ADDR + 0x0017, 0x00);
    // STA ($0,X) | 0x81 0x00
    bus.write(CARTRIDGE_START_ADDR + 0x0018, 0x81);
    bus.write(CARTRIDGE_START_ADDR + 0x0019, 0x00);
    // LDA #$30 | 0xA9 0x30
    bus.write(CARTRIDGE_START_ADDR + 0x001A, 0xA9);
    bus.write(CARTRIDGE_START_ADDR + 0x001B, 0x30);
    // STA $0073 | 0x8D 0x73 0x00
    bus.write(CARTRIDGE_START_ADDR + 0x001C, 0x8D);
    bus.write(CARTRIDGE_START_ADDR + 0x001D, 0x73);
    bus.write(CARTRIDGE_START_ADDR + 0x001E, 0x00);
    // STA ($73), Y | 0x91 0x70
    bus.write(CARTRIDGE_START_ADDR + 0x001F, 0x91);
    bus.write(CARTRIDGE_START_ADDR + 0x0020, 0x73);
}

void NES::printCycleCounts(){
    std::printf("---------Cycles--------- \nSystem cycles: %lu \nCPU cycles: %lu\n\n", cycleCount, cpu.getCycleCount());
}

void NES::setCpuFlag(CPU::StatusFlag flag, bool value){
    cpu.setFlag(flag, value);
}

bool NES::getCpuFlag(CPU::StatusFlag flag) const{
    return cpu.getFlag(flag);
}

uint8_t NES::getCpuCycleCount() const{
    return cpu.getCycleCount();
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