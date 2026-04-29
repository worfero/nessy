#include "../include/bus.h"

#include <cstdio>

Bus::Bus(){

}

void Bus::connectPPU(PPU *selec_ppu){
    ppu = selec_ppu;
}

void Bus::connectSRAM(RAM *selec_sram){
    sram = selec_sram;
}

void Bus::connectCartridge(Cartridge *selec_cartridge){
    cartridge = selec_cartridge;
}

uint8_t Bus::read(uint16_t address){
    if(address < PPU_START_ADDR){
        return sram->read(address & 0x07FF);
    }
    else if(address < APU_CONTROLS_START_ADDR){
        return ppu->read(address & 0x2007);
    }
    else if(address >= CARTRIDGE_START_ADDR && address <= MEMORY_MAP_SIZE){
        return cartridge->read(address);
    }

    return 0;
}

void Bus::write(uint16_t address, uint8_t data){
    if(address < PPU_START_ADDR){
        sram->write(address & 0x07FF, data);
    }
    else if(address < APU_CONTROLS_START_ADDR){
        return ppu->write(address & 0x2007, data);
    }
    else if(address >= CARTRIDGE_START_ADDR && address <= MEMORY_MAP_SIZE){
        cartridge->write(address - CARTRIDGE_START_ADDR, data);
    }
}