/*
 * c8dasm.cpp
 * Disassembler main
 */

#include <disassembler.h>
#include <iostream>

// semantic version
const char* version="1.0.0";

// help
void help()
{
    std::cout << "Chip8 Disassembler - " << version << " - aimktech" << std::endl;
    std::cout << "Syntax:" << std::endl;
    std::cout << "    c8dasm <ROM file>" << std::endl;
    std::cout << std::endl;
}


int main(int argc, char *argv[])
{
    Disassembler c8dasm;

    // no arguments provided
    if (argc == 1) {
        help();
        return 0;
    }

    try
    {
        // load the ROM
        c8dasm.loadROM(std::string(argv[1]));

        // analyze the code
        c8dasm.discover();

        // display the code
        c8dasm.render();

    } catch(std::string &e) {
        std::cerr << "An error occurred during disassembly procedure:";
        std::cerr << e << std::endl;
    }

    return 0;

}