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

    nextTileID = 0;
    nextAttribute = 0;
    lowBitPattern = 0;
    hiBitPattern = 0;

    bgPatternLowBits = 0;
    bgPatternHiBits = 0;
    bgAttributeLowBits = 0;
    bgAttributeHiBits = 0;

    paletteAddress = 0;

    cycle = 0;
    scanline = -1;

    frameComplete = false;
}

void PPU::clock(){
    if((registers.PPUMASK & 0x08) > 0){
        if(scanline >= -1 && scanline < 240 && cycle == 257){
            updateHorizontalBits();
        }

        if(scanline == -1 && cycle >= 280 && cycle < 305){
            updateVerticalBits();
        }

        if(scanline >= -1 && scanline < 240 && ((cycle > 0 && cycle <= 256) || (cycle > 320 && cycle <= 336))){
            uint16_t patternBase = (registers.PPUCTRL & 0x10) ? 0x1000 : 0x0000;
            uint8_t colorSelect = 0;

            // 8-cycle fetch routine
            switch(cycle % 8){
                case 0:
                    bgPatternLowBits = (bgPatternLowBits & 0xFF00) | lowBitPattern;
                    bgPatternHiBits = (bgPatternHiBits & 0xFF00) | hiBitPattern;

                    colorSelect = nextAttribute;
                    if(v_reg & 0x40) colorSelect >>= 4;
                    if(v_reg & 0x02) colorSelect >>= 2;
                    colorSelect &= 0x03;
                    bgAttributeLowBits = (colorSelect & 0x01) ? (bgAttributeLowBits & 0xFF00) | 0x00FF : (bgAttributeLowBits & 0xFF00) | 0x0000;
                    bgAttributeHiBits = (colorSelect & 0x02) ? (bgAttributeHiBits & 0xFF00) | 0x00FF : (bgAttributeHiBits & 0xFF00) | 0x0000;

                    incrementCoarseX();
                    break;
                    
                case 1:
                    nextTileID = read(0x2000 | (v_reg & 0x0FFF));
                    break;

                case 3:
                    nextAttribute = read(0x23C0 | (v_reg & 0x0C00) | ((v_reg >> 4) & 0x38) | ((v_reg >> 2) & 0x07));
                    break;
                
                case 5:
                    lowBitPattern = read(patternBase + (nextTileID * 16) + FINE_Y);
                    break;

                case 7:
                    hiBitPattern = read(patternBase + (nextTileID * 16) + FINE_Y + 8);
                    break;

                default:
                    break;
            }

            if(cycle == 256) incrementFineY();

            uint16_t bitMux = 0x8000 >> x_reg;

            uint8_t p0 = (bgPatternLowBits & bitMux) ? 1 : 0;
            uint8_t p1 = (bgPatternHiBits  & bitMux) ? 1 : 0;
            uint8_t a0 = (bgAttributeLowBits & bitMux) ? 1 : 0;
            uint8_t a1 = (bgAttributeHiBits  & bitMux) ? 1 : 0;

            uint8_t palette = (a1 << 1) | a0;
            uint8_t pixel = (p1 << 1) | p0;

            if(pixel == 0){
                paletteAddress = PALETTE_START_ADDR;
            }
            else{
                paletteAddress = PALETTE_START_ADDR + (palette * 4) + pixel;
            }

            if(scanline >= 0 && scanline < 240 && cycle > 0 && cycle <= 256){
                uint32_t pixelColor = nesPalette[read(paletteAddress) & 0x3F];
                framebuffer.at((scanline * 256) + cycle - 1) = pixelColor;   
            }

            bgAttributeLowBits <<= 1;
            bgAttributeHiBits <<= 1;
            bgPatternLowBits <<= 1;
            bgPatternHiBits <<= 1;
        }
    }

    cycle++;

    if(cycle == 341){
        cycle = 0;
        scanline++;

        if(scanline == 261) scanline = -1;
    }

    if(scanline == 241 && cycle == 1) {
        setFlag(VBLANK, 1);
        if((registers.PPUCTRL & 0x80) != 0){
            cpu->requestNMI();
        }

        frameComplete = true;
    }

    if(scanline == -1 && cycle == 1) setFlag(VBLANK, 0);
}

void PPU::updateHorizontalBits(){
    // clear the 
    v_reg = (v_reg & 0x7BE0) | (t_reg & 0x041F);
}

void PPU::updateVerticalBits(){
    v_reg = (v_reg & 0x841F) | (t_reg & 0x7BE0);
}

void PPU::connectCartridge(Cartridge *selec_cartridge){
    cartridge = selec_cartridge;
}

void PPU::connectCPU(CPU *selec_cpu){
    cpu = selec_cpu;
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

void PPU::incrementCoarseX(){
    if((v_reg & 0x001F) == 0x001F){
        v_reg &= 0xFFE0;
        v_reg ^= 0x0400;
    }
    else{
        v_reg++;
    }
}

void PPU::incrementFineY(){
    if((v_reg & 0x7000) == 0x7000){
        v_reg &= 0x8FFF;
        incrementCoarseY();
    }
    else{
        v_reg += 0x1000;
    }
}

void PPU::incrementCoarseY(){
    if((v_reg & 0x03E0) == 0x03A0){
        v_reg &= 0xFC1F;
        v_reg ^= 0x0800;
    }
    else if((v_reg & 0x03E0) == 0x03E0){
        v_reg &= 0xFC1F;
    }
    else{
        v_reg += 0x0020;
    }
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

uint32_t *PPU::getFramebuffer(){
    return framebuffer.data();
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

uint16_t PPU::getCycle() const{
    return cycle;
}

int16_t PPU::getScanline() const{
    return scanline;
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

void PPU::printRegisters(){
    std::printf("--------PPU Registers-------- \nPPUCTRL: 0x%02X \nPPUMASK: 0x%02X \nPPUSTATUS: 0x%02X \nOAMADDR: 0x%02X \nOAMDATA: 0x%02X \nPPUSCROLL: 0x%02X\nPPUADDR: 0x%02X\nPPUDATA: 0x%02X\nOAMDMA: 0x%02X\n\n",
                registers.PPUCTRL, registers.PPUMASK, registers.PPUSTATUS, registers.OAMADDR, registers.OAMDATA, registers.PPUSCROLL,
                registers.PPUADDR, registers.PPUDATA, registers.OAMDMA);
}

void PPU::printState(){
    std::printf("-------PPU State------- \nW: %d \nT: 0x%04X \nV: 0x%04X \nX: 0x%02X \n\n", getW(), getT(), getV(), getX());
}

void PPU::printCurrentTile(){
    std::printf("-------PPU Tiles------- \nnextTileID: 0x%02X \nnextAttribute: 0x%02X \nlowBitPattern: 0x%02X \nhiBitPattern: 0x%02X\npaletteAddr: 0x%04X \n\n",
                nextTileID, nextAttribute, lowBitPattern, hiBitPattern, paletteAddress);
}