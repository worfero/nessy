#include "../include/cartridge.h"

#include <cstdio>

Cartridge::Cartridge(){

}

bool Cartridge::loadRomFile(const std::string &filename){
    std::ifstream file(filename, std::ios::binary);

    if(!file){
        std::printf("\nError: ROM file could not be opened.\n");
        return false;
    }

    uint8_t header[16];
    file.read((char*)header, 16);

    if(header[0] != 'N' || header[1] != 'E' || header[2] != 'S'){
        std::printf("\nError: ROM header invalid.\n");
        return false;
    }

    uint8_t prgRomSize = (uint8_t)header[4];

    memory.resize(prgRomSize * 0x4000);
    file.read((char*)memory.data(), memory.size());

    return true;
}

uint8_t Cartridge::read(uint16_t address){
    if(address >= 0x8000){
        address -= 0x8000;

        // mirroring
        if(memory.size() == 0x4000){
            address &= 0x3FFF;
        }

        return memory.at(address);
    }

    return 0;
}

void Cartridge::write(uint16_t address, uint8_t data){
    if(address >= 0x8000){
        address -= 0x8000;

        // mirroring
        if(memory.size() == 0x4000){
            address &= 0x3FFF;
        }

        memory.at(address) = data;
    }
}

void Cartridge::printMemoryMap(uint16_t startAddr, uint16_t rows){
    const uint16_t bytesPerRow = 8;
    uint16_t baseAddress = startAddr & 0xFFF8; // row allignment

    for(uint16_t row = 0; row < rows; row++){
        if(baseAddress < memory.size()){
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