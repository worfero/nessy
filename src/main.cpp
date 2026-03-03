#include "../include/NES.h"

int main(){
    NES nes;

    nes.printMemoryMap(0x0FFF, 2);

    return 0;
}