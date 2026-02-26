#include "../include/bus.h"
#include "../include/sram.h"

#include <cstdio>

Bus::Bus(){

}

void Bus::connectSRAM(SRAM *selec_sram){
    sram = selec_sram;
}

uint8_t Bus::read(uint16_t address){
    if(address <= 0x1FFF){
        return sram->read(address);
    }

    return 0;
}

void Bus::write(uint16_t address, uint8_t data){
    if(address <= 0x1FFF){
        sram->write(address, data);
    }
}