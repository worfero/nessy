# nessy
This is a simple NES emulator I've been building solely for C++ practicing and digital architecture studying. I do not intend to release it as a serious, qualified software.

The CPU is not cycle-accurate, it rather executes the whole instruction in one cycle and then clocks the remaining cycles doing nothing. For now, it can perform every official opcode from the original instruction set.

The PPU is still under development and the controllers have not been implemented yet.