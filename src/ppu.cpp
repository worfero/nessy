#include "../include/ppu.h"

PPU::PPU(){
    // registers default state
    registers.PPUCTRL = registers.PPUMASK = registers.PPUSTATUS = registers.OAMADDR = registers.OAMDATA = registers.PPUSCROLL =
    registers.PPUADDR = registers.PPUDATA = registers.OAMDMA = 0;

    w_latch = false;
    t_reg = 0x0000;
    v_reg = 0x0000;
    x_reg = 0x00;
    readBuffer = 0x00;
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
                w_latch = false;
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
                if(v_reg >= PALETTE_START_ADDR){
                    value = read(v_reg);
                    readBuffer = read(v_reg - 0x1000);
                }
                else{
                    value = readBuffer;
                    readBuffer = read(v_reg);
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

            if(!w_latch){
                x_reg = registers.PPUSCROLL & 0x07;
                t_reg = (t_reg & 0xFFE0) | (registers.PPUSCROLL >> 3);
                w_latch = true;
            }
            else{
                t_reg = (t_reg & 0x8FFF) | ((registers.PPUSCROLL & 0x07) << 12);
                t_reg = (t_reg & 0xFC1F) | ((registers.PPUSCROLL & 0xF8) << 2);
                w_latch = false;
            }
            break;
        case 0x2006:
            registers.PPUADDR = data;

            if(!w_latch){
                t_reg = (t_reg & 0x00FF) | ((registers.PPUADDR & 0x3F) << 8);
                w_latch = true;
            }
            else{
                t_reg = (t_reg & 0xFF00) | registers.PPUADDR;
                v_reg = t_reg;
                w_latch = false;
            }
            break;
        case 0x2007:
            registers.PPUDATA = data;
            write(v_reg, registers.PPUDATA);
            incrementFetchAddr();
            break;

        default:
            break;
    }
}

void PPU::incrementFetchAddr(){
    if(registers.PPUCTRL & 0x04){
        v_reg += 0x20;
    }
    else{
        v_reg++;
    }

    v_reg &= 0x3FFF;
}

uint8_t PPU::read(uint16_t address){
    if(address >= CHR_ROM_START_ADDR && address < NAMETABLE_START_ADDR){
        return cartridge->readChr(address);
    }
    else if(address < PALETTE_START_ADDR){
        if(address >= 0x3000) address -= 0x1000;
        return vram.read((address - NAMETABLE_START_ADDR) & 0x07FF);
    }
    else if(address < MIRROR_START_ADDR){
        address = (address - PALETTE_START_ADDR) & 0x001F;

        // palette mirroring
        if (address == 0x0010) address = 0x0000;
        if (address == 0x0014) address = 0x0004;
        if (address == 0x0018) address = 0x0008;
        if (address == 0x001C) address = 0x000C;

        return paletteRam.at(address);
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
    else if(address < MIRROR_START_ADDR){
        address = (address - PALETTE_START_ADDR) & 0x001F;

        // palette mirroring
        if (address == 0x10) address = 0x00;
        if (address == 0x14) address = 0x04;
        if (address == 0x18) address = 0x08;
        if (address == 0x1C) address = 0x0C;

        paletteRam.at(address) = data;
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

bool PPU::getW() const{
    return w_latch;
}

uint16_t PPU::getT() const{
    return t_reg;
}

uint16_t PPU::getV() const{
    return v_reg;
}

uint8_t PPU::getX() const{
    return x_reg;
}

void PPU::printState(){
    std::printf("-------PPU State------- \nW: %d \nT: 0x%04X \nV: 0x%04X \nX: 0x%02X \n\n", getW(), getT(), getV(), getX());
}