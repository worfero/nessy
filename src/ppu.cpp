#include "../include/ppu.h"

PPU::PPU(){
    // registers default state
    registers.PPUCTRL = registers.PPUMASK = registers.PPUSTATUS = registers.OAMADDR = registers.OAMDATA = registers.PPUSCROLL =
    registers.PPUADDR = registers.PPUDATA = registers.OAMDMA = 0;

    ppuAddrLatch = false;
    tempFetchAddr = 0x0000;
    fetchAddr = 0x0000;
    readBuffer = 0x0000;
}

void PPU::connectCartridge(Cartridge *selec_cartridge){
    cartridge = selec_cartridge;
}

uint8_t PPU::readReg(uint16_t addr){
    switch(addr){
        case 0x2000:
            return registers.PPUCTRL;
        case 0x2001:
            return registers.PPUMASK;
        case 0x2002:
            {
                uint8_t sts = registers.PPUSTATUS;
                setFlag(VBLANK, false);
                ppuAddrLatch = false;
                return sts;
            }
        case 0x2003:
            return registers.OAMADDR;
        case 0x2004:
            return registers.OAMDATA;
        case 0x2005:
            return registers.PPUSCROLL;
        case 0x2006:
            return registers.PPUADDR;
        case 0x2007:
            {
                uint8_t value;
                if(fetchAddr >= PALETTE_START_ADDR){
                    value = read(fetchAddr);
                    readBuffer = read(fetchAddr - 0x1000);
                }
                else{
                    value = readBuffer;
                    readBuffer = read(fetchAddr);
                }
                incrementFetchAddr();
                return value;
            }

        default:
            return 0;
    }
}

void PPU::writeReg(uint16_t addr, uint8_t data){
    switch(addr){
        case 0x2000:
            registers.PPUCTRL = data;
            break;
        case 0x2001:
            registers.PPUMASK = data;
            break;
        case 0x2002:
            // read-only
            break;
        case 0x2003:
            registers.OAMADDR = data;
            break;
        case 0x2004:
            registers.OAMDATA = data;
            break;
        case 0x2005:
            registers.PPUSCROLL = data;
            break;
        case 0x2006:
            registers.PPUADDR = data;

            if(!ppuAddrLatch){
                tempFetchAddr = registers.PPUADDR << 8;
                ppuAddrLatch = true;
            }
            else{
                fetchAddr = (tempFetchAddr | registers.PPUADDR) & 0x3FFF;
                ppuAddrLatch = false;
            }
            break;
        case 0x2007:
            registers.PPUDATA = data;
            write(fetchAddr, registers.PPUDATA);
            incrementFetchAddr();
            break;

        default:
            break;
    }
}

void PPU::incrementFetchAddr(){
    if(registers.PPUCTRL & 0x04){
        fetchAddr += 0x20;
    }
    else{
        fetchAddr++;
    }

    fetchAddr &= 0x3FFF;
}

uint8_t PPU::read(uint16_t address){
    if(address >= CHR_ROM_START_ADDR && address < NAMETABLE_START_ADDR){
        return cartridge->readChr(address);
    }
    else if(address < PALETTE_START_ADDR){
        if(address >= 0x3000) address -= 0x1000;
        return vram.read((address - NAMETABLE_START_ADDR) & 0x07FF);
    }

    return 0;
}

void PPU::write(uint16_t address, uint8_t data){
    if(address >= CHR_ROM_START_ADDR && address < NAMETABLE_START_ADDR){
        cartridge->writeChr(address, data);
    }
    else if(address < PALETTE_START_ADDR){
        if(address >= 0x3000) address -= 0x1000;
        vram.write((address - NAMETABLE_START_ADDR) & 0x07FF, data);
    }
}

void PPU::setFlag(StatusFlag flag, bool value){
    if (value)
        registers.PPUSTATUS |= flag;
    else
        registers.PPUSTATUS &= ~flag;
}

bool PPU::getFlag(StatusFlag flag) const{
    return (registers.PPUSTATUS & flag) != 0;
}