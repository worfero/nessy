#include "../include/ram.h"

#include <cstdio>

RAM::RAM(){

}

uint8_t RAM::read(uint16_t address){
    return memory[address & 0x07FF];
}

void RAM::write(uint16_t address, uint8_t data){
    memory[address & 0x07FF] = data;
}

void RAM::printMemoryMap(uint16_t startAddr, uint16_t rows){
    const uint16_t bytesPerRow = 8;
    uint16_t baseAddress = startAddr & 0xFFF8; // row allignment

    for(uint16_t row = 0; row < rows; row++){
        if(baseAddress < RAM_SIZE){
            std::printf("0x%04X: ", baseAddress);
            for(uint16_t idx = 0; idx < bytesPerRow; idx++){
                std::printf("0x%02X ", memory[baseAddress + idx]);
            }
            std::printf("\n");
        } else{
            break;
        }

        baseAddress += bytesPerRow;
    }
    std::printf("\n");
}