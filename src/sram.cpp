#include "../include/sram.h"

#include <cstdio>

SRAM::SRAM(){

}

uint8_t SRAM::read(uint16_t address){
    return memory[address & 0x07FF];
}

void SRAM::write(uint16_t address, uint8_t data){
    memory[address & 0x07FF] = data;
}

void SRAM::printMemoryMap(){
    for(auto value : memory){
        std::printf("%u ", value);
    }
}