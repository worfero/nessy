#include "../include/ppu.h"

PPU::PPU(){
    // registers default state
    registers.PPUCTRL = registers.PPUMASK = registers.PPUSTATUS = registers.OAMADDR = registers.OAMDATA = registers.PPUSCROLL =
    registers.PPUADDR = registers.PPUDATA = registers.OAMDMA = 0;

    ppuAddrLatch = false;
    tempFetchAddr = 0x0000;
    fetchAddr = 0x0000;
}

void PPU::connectCartridge(Cartridge *selec_cartridge){
    cartridge = selec_cartridge;
}

uint8_t PPU::readReg(uint16_t addr){
    std::printf("\n\nRead\n\n");
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
                uint8_t value = readVram(fetchAddr);
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
                std::printf("\n\n%u\n\n", fetchAddr);
                fetchAddr = (tempFetchAddr | registers.PPUADDR) & 0x3FFF;
                ppuAddrLatch = false;
            }
            break;
        case 0x2007:
            registers.PPUDATA = data;
            writeVram(fetchAddr, registers.PPUDATA);
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

uint8_t PPU::readVram(uint16_t address){
    if(address >= NAMETABLE_START_ADDR && address < PALETTE_START_ADDR){
        return vram.read((address - NAMETABLE_START_ADDR) & 0x07FF);
    }

    return 0;
}

void PPU::writeVram(uint16_t address, uint8_t data){
    if(address >= NAMETABLE_START_ADDR && address < PALETTE_START_ADDR){
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