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

uint8_t CPU::LDA(){
    registers.A = fetchValue();
    setFlag(ZERO, registers.A == 0);
    setFlag(NEGATIVE, registers.A & 0x80);
    return 1;
}

uint8_t CPU::LDX(){
    registers.X = fetchValue();
    setFlag(ZERO, registers.X == 0);
    setFlag(NEGATIVE, registers.X & 0x80);
    return 1;
}

uint8_t CPU::LDY(){
    registers.Y = fetchValue();
    setFlag(ZERO, registers.Y == 0);
    setFlag(NEGATIVE, registers.Y & 0x80);
    return 1;
}

uint8_t CPU::STA(){
    bus.write(fetchAddr, registers.A);
    return 0;
}

uint8_t CPU::STX(){
    bus.write(fetchAddr, registers.X);
    return 0;
}

uint8_t CPU::STY(){
    bus.write(fetchAddr, registers.Y);
    return 0;
}

uint8_t CPU::TAX(){
    registers.X = registers.A;
    setFlag(ZERO, registers.X == 0);
    setFlag(NEGATIVE, registers.X & 0x80);
    return 0;
}

uint8_t CPU::TAY(){
    registers.Y = registers.A;
    setFlag(ZERO, registers.Y == 0);
    setFlag(NEGATIVE, registers.Y & 0x80);
    return 0;
}

uint8_t CPU::TXA(){
    registers.A = registers.X;
    setFlag(ZERO, registers.A == 0);
    setFlag(NEGATIVE, registers.A & 0x80);
    return 0;
}

uint8_t CPU::TYA(){
    registers.A = registers.Y;
    setFlag(ZERO, registers.A == 0);
    setFlag(NEGATIVE, registers.A & 0x80);
    return 0;
}

uint8_t CPU::INX(){
    // uint8_t automatically wraps 0xFF to 0x00
    registers.X++;
    setFlag(ZERO, registers.X == 0);
    setFlag(NEGATIVE, registers.X & 0x80);

    return 0;
}

uint8_t CPU::INY(){
    // uint8_t automatically wraps 0xFF to 0x00
    registers.Y++;
    setFlag(ZERO, registers.Y == 0);
    setFlag(NEGATIVE, registers.Y & 0x80);

    return 0;
}

uint8_t CPU::DEX(){
    // uint8_t automatically wraps 0x00 to 0xFF
    registers.X--;
    setFlag(ZERO, registers.X == 0);
    setFlag(NEGATIVE, registers.X & 0x80);

    return 0;
}

uint8_t CPU::DEY(){
    // uint8_t automatically wraps 0x00 to 0xFF
    registers.Y--;
    setFlag(ZERO, registers.Y == 0);
    setFlag(NEGATIVE, registers.Y & 0x80);

    return 0;
}

void CPU::fillOpcodes(){
    instructions.fill({"XXX", &CPU::XXX, &CPU::IMP, 2});

    instructions[0x81] = {"STA", &CPU::STA, &CPU::IZX, 6};
    instructions[0x84] = {"STY", &CPU::STY, &CPU::ZP, 3};
    instructions[0x85] = {"STA", &CPU::STA, &CPU::ZP, 3};
    instructions[0x86] = {"STX", &CPU::STX, &CPU::ZP, 3};
    instructions[0x88] = {"DEY", &CPU::DEY, &CPU::IMP, 2};
    instructions[0x8A] = {"TXA", &CPU::TXA, &CPU::IMP, 2};
    instructions[0x8C] = {"STY", &CPU::STY, &CPU::ABS, 4};
    instructions[0x8D] = {"STA", &CPU::STA, &CPU::ABS, 4};
    instructions[0x8E] = {"STX", &CPU::STX, &CPU::ABS, 4};
    instructions[0x91] = {"STA", &CPU::STA, &CPU::IZY, 6};
    instructions[0x94] = {"STY", &CPU::STY, &CPU::ZPX, 4};
    instructions[0x95] = {"STA", &CPU::STA, &CPU::ZPX, 4};
    instructions[0x96] = {"STX", &CPU::STX, &CPU::ZPY, 4};
    instructions[0x98] = {"TYA", &CPU::TYA, &CPU::IMP, 2};
    instructions[0x99] = {"STA", &CPU::STA, &CPU::ABY, 5};
    instructions[0x9D] = {"STA", &CPU::STA, &CPU::ABX, 5};
    instructions[0xA0] = {"LDY", &CPU::LDY, &CPU::IMM, 2};
    instructions[0xA1] = {"LDA", &CPU::LDA, &CPU::IZX, 6};
    instructions[0xA2] = {"LDX", &CPU::LDX, &CPU::IMM, 2};
    instructions[0xA4] = {"LDY", &CPU::LDY, &CPU::ZP, 3};
    instructions[0xA5] = {"LDA", &CPU::LDA, &CPU::ZP, 3};
    instructions[0xA6] = {"LDX", &CPU::LDX, &CPU::ZP, 3};
    instructions[0xA8] = {"TAY", &CPU::TAY, &CPU::IMP, 2};
    instructions[0xA9] = {"LDA", &CPU::LDA, &CPU::IMM, 2};
    instructions[0xAA] = {"TAX", &CPU::TAX, &CPU::IMP, 2};
    instructions[0xAC] = {"LDY", &CPU::LDY, &CPU::ABS, 4};
    instructions[0xAD] = {"LDA", &CPU::LDA, &CPU::ABS, 4};
    instructions[0xAE] = {"LDX", &CPU::LDX, &CPU::ABS, 4};
    instructions[0xB1] = {"LDA", &CPU::LDA, &CPU::IZY, 5};
    instructions[0xB4] = {"LDY", &CPU::LDY, &CPU::ZPX, 4};
    instructions[0xB5] = {"LDA", &CPU::LDA, &CPU::ZPX, 4};
    instructions[0xB6] = {"LDX", &CPU::LDX, &CPU::ZPY, 4};
    instructions[0xB9] = {"LDA", &CPU::LDA, &CPU::ABY, 4};
    instructions[0xBC] = {"LDY", &CPU::LDY, &CPU::ABX, 4};
    instructions[0xBD] = {"LDA", &CPU::LDA, &CPU::ABX, 4};
    instructions[0xBE] = {"LDX", &CPU::LDX, &CPU::ABY, 4};
    instructions[0xC8] = {"INY", &CPU::INY, &CPU::IMP, 2};
    instructions[0xCA] = {"DEX", &CPU::DEX, &CPU::IMP, 2};
    instructions[0xE8] = {"INX", &CPU::INX, &CPU::IMP, 2};
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
    std::printf("--------Registers-------- \nA: 0x%02X \nX: 0x%02X \nY: 0x%02X \nSP: 0x%02X \nP: 0x%02X \nPC: 0x%04X\n\n",
                registers.A, registers.X, registers.Y, registers.SP, registers.P, registers.PC);
}

void CPU::printFlags() const{
    std::printf("------Status Flags------ \nCARRY: %d \nZERO: %d \nINTERRUPT: %d \nDECIMAL: %d \nBREAK: %d \nUNUSED: %d \nOVERFLOW: %d \nNEGATIVE: %d \n\n",
                getFlag(CARRY), getFlag(ZERO), getFlag(INTERRUPT), getFlag(DECIMAL), getFlag(BREAK), getFlag(UNUSED), getFlag(OVERFLOW), getFlag(NEGATIVE));
}