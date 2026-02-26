#include "../include/cpu.h"
#include "../include/bus.h"
#include <cstdio>

CPU::CPU(){
    // registers default state
    registers.A = registers.X = registers.Y = 0;
    registers.SP = 0xFD;      // starting stack pointer
    registers.P  = 0x24;      // 0010 0100 - interrupt disabled and unused bit set
    registers.PC = 0;
}

void CPU::reset(){
    registers.A = registers.X = registers.Y = 0;
    registers.SP = 0xFD;
    registers.P  = 0x24;
    registers.PC = 0x0000;
}

void CPU::setFlag(StatusFlag flag, bool value){
    if (value)
        registers.P |= flag;
    else
        registers.P &= ~flag;
}

bool CPU::getFlag(StatusFlag flag) const{
    return (registers.P & flag) != 0;
}

void CPU::connectBus(Bus *selec_bus){
    bus = selec_bus;
}

uint8_t CPU::readBus(uint16_t address){
    return bus->read(address);
}

void CPU::writeBus(uint16_t address, uint8_t data){
    bus->write(address, data);
}

void CPU::printRegisters() const{
    std::printf("--------Registers-------- \nA:%02X \nX:%02X \nY:%02X \nSP:%02X \nP:%02X \nPC:%04X\n\n",
                registers.A, registers.X, registers.Y, registers.SP, registers.P, registers.PC);
}