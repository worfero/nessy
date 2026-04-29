#include "../include/ppu.h"

#include <cstdio>

PPU::PPU(){

}

void PPU::connectCartridge(Cartridge *selec_cartridge){
    cartridge = selec_cartridge;
}

uint8_t PPU::read(uint16_t address){
    if(address >= NAMETABLE_START_ADDR && address < PALETTE_START_ADDR){
        return vram.read((address - NAMETABLE_START_ADDR) & 0x07FF);
    }

    return 0;
}

void PPU::write(uint16_t address, uint8_t data){
    if(address >= NAMETABLE_START_ADDR && address < PALETTE_START_ADDR){
        vram.write((address - NAMETABLE_START_ADDR) & 0x07FF, data);
    }
}