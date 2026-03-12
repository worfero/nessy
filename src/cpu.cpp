#include "../include/cpu.h"
#include "../include/bus.h"
#include <cstdio>

CPU::CPU(Bus &selec_bus) : bus(selec_bus){
    // registers default state
    registers.A = registers.X = registers.Y = 0;
    registers.SP = 0xFD;      // starting stack pointer
    registers.P  = 0x24;      // 0010 0100 - interrupt disabled and unused bit set
    registers.PC = 0x8000;    // default PGR-ROM start address

    fillOpcodes();
}

uint8_t CPU::XXX(){
    std::printf("Illegal opcode detected.\n\n");
    return 0;
}

uint8_t CPU::LDA(){
    registers.A = fetchValue();
    setFlag(ZERO, registers.A == 0);
    setFlag(NEGATIVE, registers.A & 0x80);
    return 1;
}

uint8_t CPU::IMP(){
    return 0;
}

uint8_t CPU::IMM(){
    fetchAddr = registers.PC;
    registers.PC++;
    return 0;
}

uint8_t CPU::ACC(){
    return 0;
}

uint8_t CPU::ZP(){
    fetchAddr = bus.read(registers.PC++);
    fetchAddr &= 0x00FF;
    return 0;
}

uint8_t CPU::ZPX(){
    fetchAddr = bus.read(registers.PC++);
    fetchAddr = (fetchAddr + registers.X) & 0x00FF;
    return 0;
}

uint8_t CPU::ZPY(){
    fetchAddr = bus.read(registers.PC++);
    fetchAddr = (fetchAddr + registers.Y) & 0x00FF;
    return 0;
}

uint8_t CPU::ABS(){
    uint16_t loByte = bus.read(registers.PC++);
    uint16_t hiByte = bus.read(registers.PC++);
    fetchAddr = (hiByte << 8) | loByte;
    return 0;
}

uint8_t CPU::ABX(){
    uint16_t loByte = bus.read(registers.PC++);
    uint16_t hiByte = bus.read(registers.PC++);

    uint16_t baseAddr = (hiByte << 8) | loByte;
    fetchAddr = baseAddr + registers.X;

    if((fetchAddr & 0xFF00) != (baseAddr & 0xFF00)){
        return 1;
    }

    return 0;
}

uint8_t CPU::ABY(){
    uint16_t loByte = bus.read(registers.PC++);
    uint16_t hiByte = bus.read(registers.PC++);

    uint16_t baseAddr = (hiByte << 8) | loByte;
    fetchAddr = baseAddr + registers.Y;

    if((fetchAddr & 0xFF00) != (baseAddr & 0xFF00)){
        return 1;
    }

    return 0;
}

uint8_t CPU::IND(){
    uint16_t ptrLoByte = bus.read(registers.PC++);
    uint16_t ptrHiByte = bus.read(registers.PC++);
    uint16_t ptr = (ptrHiByte << 8) | ptrLoByte;

    uint16_t loByte = bus.read(ptr);
    uint16_t hiByte;

    // 6502 page boundary bug replication
    if(ptrLoByte == 0xFF){
        hiByte = bus.read(ptr & 0xFF00);
    }
    else{
        hiByte = bus.read(ptr + 1);
    }

    fetchAddr = (hiByte << 8) | loByte;

    return 0;
}

uint8_t CPU::IZX(){
    uint16_t ptr = bus.read(registers.PC++);
    ptr = (ptr + registers.X) & 0x00FF;

    uint16_t loByte = bus.read(ptr++);
    uint16_t hiByte = bus.read(ptr & 0x00FF);

    fetchAddr = (hiByte << 8) | loByte;

    return 0;
}

uint8_t CPU::IZY(){
    uint16_t ptr = bus.read(registers.PC++);

    uint16_t loByte = bus.read(ptr & 0x00FF);
    uint16_t hiByte = bus.read((ptr + 1) & 0x00FF);
    uint16_t baseAddr = (hiByte << 8) | loByte;

    fetchAddr = baseAddr + registers.Y;

    if((fetchAddr & 0xFF00) != (baseAddr & 0xFF00)){
        return 1;
    }

    return 0;
}

uint8_t CPU::REL(){
    int8_t addrOffset = bus.read(registers.PC++);
    fetchAddr = registers.PC + addrOffset;

    return 0;
}

void CPU::fillOpcodes(){
    instructions.fill({"XXX", &CPU::XXX, &CPU::IMP, 2});

    instructions[0xA9] = {"LDA", &CPU::LDA, &CPU::IMM, 2};
}

uint8_t CPU::fetchValue(){
    if (!(instructions[opcode].addrMode == &CPU::IMP))
        fetchedValue = bus.read(fetchAddr);
    return fetchedValue;
}

void CPU::clock(){
    // cycle countdown method, useful for timing but not precise to reality
    if(instructionCycles == 0){
        opcode = bus.read(registers.PC++);
        Instruction &instruction = instructions[opcode];

        instructionCycles = instruction.cycleCount;

        uint8_t extraCycleCheck1 = (this->*instruction.addrMode)();
        uint8_t extraCycleCheck2 = (this->*instruction.operation)();

        instructionCycles += (extraCycleCheck1 & extraCycleCheck2);
    }

    instructionCycles--;
    cycleCount++;
}

void CPU::reset(){
    registers.A = registers.X = registers.Y = 0;
    registers.SP = 0xFD;
    registers.P  = 0x24;

    uint16_t reset_lo = bus.read(0xFFFC);
    uint16_t reset_hi = bus.read(0xFFFD);
    registers.PC = (reset_hi << 8) | reset_lo;

    fetchedValue = 0;
    fetchAddr = 0;

    instructionCycles = 0;
    opcode = 0;
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

uint8_t CPU::getInstructionCycleCounter() const{
    return instructionCycles;
}

uint64_t CPU::getCycleCount() const{
    return cycleCount;
}

uint8_t CPU::readBus(uint16_t address){
    return bus.read(address);
}

void CPU::writeBus(uint16_t address, uint8_t data){
    bus.write(address, data);
}

void CPU::printRegisters() const{
    std::printf("--------Registers-------- \nA:%02X \nX:%02X \nY:%02X \nSP:%02X \nP:%02X \nPC:%04X\n\n",
                registers.A, registers.X, registers.Y, registers.SP, registers.P, registers.PC);
}

void CPU::printFlags() const{
    std::printf("------Status Flags------ \nCARRY: %d \nZERO: %d \nINTERRUPT: %d \nDECIMAL: %d \nBREAK: %d \nUNUSED: %d \nOVERFLOW: %d \nNEGATIVE: %d \n\n",
                getFlag(CARRY), getFlag(ZERO), getFlag(INTERRUPT), getFlag(DECIMAL), getFlag(BREAK), getFlag(UNUSED), getFlag(OVERFLOW), getFlag(NEGATIVE));
}