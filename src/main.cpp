/*
 * main.cpp
 * Main entry point for Chip8+ emulator
 */

// includes
#include <iostream>
#include <exception>
#include "vm.h"

// semantic version
const char* version="1.0.0";

// functions
void help()
{
    std::cout << "chip8 emulator - " << version << " - aimktech" << std::endl;
    std::cout << "Syntax:" << std::endl;
    std::cout << "    chip8 <ROM file>" << std::endl;
    std::cout << std::endl;
    std::cout << "Another Chip8 emulator written in C++." << std::endl;
    std::cout << std::endl;
}

void printInfo()
{
    std::cout << "chip8 emulator - " << version << " - aimktech" << std::endl;
    std::cout << "ESC : exit emulator" << std::endl;
    std::cout << "F1  : reduce emulator speed" << std::endl;
    std::cout << "F2  : reset emulator speed" << std::endl;
    std::cout << "F3  : increase emulator speed" << std::endl;
    std::cout << "F10 : restart emulator" << std::endl;
    std::cout << "P   : pause the emulator" << std::endl;
}

// main entry point
int main(int argc, char* argv[])
{
    // no arguments provided
    if (argc == 1) {
        help();
        return 0;
    }

    printInfo();

    try
    {
        VM myVM;

        // initialize the Virtual Machine
        myVM.init();

        // load the ROM
        myVM.loadRom(argv[1]);

        // run the VM
        myVM.run();

        // shutdown the VM
        myVM.shutdown();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}