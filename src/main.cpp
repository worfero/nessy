#include "../include/cpu.h"

int main() {
    CPU cpu;

    cpu.reset();
    cpu.print_registers();

    cpu.setFlag(CPU::CARRY, true);
    cpu.print_registers();

    return 0;
}