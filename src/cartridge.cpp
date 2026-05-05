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

    // load pgrRom
    uint8_t prgRomSize = (uint8_t)header[4];
    pgrRom.resize(prgRomSize * 0x4000);
    file.read((char*)pgrRom.data(), pgrRom.size());

    // load chrRom
    uint8_t chrRomSize = (uint8_t)header[5];

    if(chrRomSize == 0){
        allowChrWrite = true;
        chrRom.assign(0x2000, 0x00);
    }
    else{
        allowChrWrite = false;
        chrRom.resize(chrRomSize * 0x2000);
        file.read((char*)chrRom.data(), chrRom.size());
    }

    return true;
}

uint8_t Cartridge::readPgr(uint16_t address){
    if(address >= 0x8000){
        address -= 0x8000;

        // mirroring
        if(pgrRom.size() == 0x4000){
            address &= 0x3FFF;
        }

        return pgrRom.at(address);
    }

    return 0;
}

uint8_t Cartridge::readChr(uint16_t address){
    if(address < 0x2000){
        return chrRom.at(address);
    }

    return 0;
}

void Cartridge::writeChr(uint16_t address, uint8_t data){
    if(address < 0x2000 && allowChrWrite){
        chrRom.at(address) = data;
    }
}