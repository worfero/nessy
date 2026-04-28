#include "../include/bus.h"

#include <cstdio>

Bus::Bus(){

}

void Bus::connectSRAM(RAM *selec_sram){
    sram = selec_sram;
}

void Bus::connectCartridge(Cartridge *selec_cartridge){
    cartridge = selec_cartridge;
}

uint8_t Bus::read(uint16_t address){
    if(address <= 0x1FFF){
        return sram->read(address);
    }
    else if(address >= CARTRIDGE_START_ADDR && address <= MEMORY_MAP_SIZE){
        return cartridge->read(address);
    }

    return 0;
}

void Bus::write(uint16_t address, uint8_t data){
    if(address <= 0x1FFF){
        sram->write(address, data);
    }
    
    else if(address >= CARTRIDGE_START_ADDR && address <= MEMORY_MAP_SIZE){
        cartridge->write(address - CARTRIDGE_START_ADDR, data);
    }
}