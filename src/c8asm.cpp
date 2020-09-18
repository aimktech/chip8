/*
 * c8dasm.cpp
 * Disassembler main
 */

#include <iostream>
#include "assembler.h"

// semantic version
const char* version="1.0.0";

// help
void help()
{
    std::cout << "Chip8 Assembler - " << version << " - aimktech" << std::endl;
    std::cout << "Syntax:" << std::endl;
    std::cout << "    c8asm <ASM file> <output>" << std::endl;
    std::cout << std::endl;
}


int main(int argc, char *argv[])
{
    Assembler c8asm;

    // no arguments provided
    if (argc <= 2) {
        help();
        return 0;
    }

    try
    {
        // compile & write
        if( c8asm.compile(std::string(argv[1])) ) {
            c8asm.write(std::string(argv[2]));
        }
    } catch(std::string &e) {
        std::cerr << "An error occurred during assembly procedure:";
        std::cerr << e << std::endl;
    }

    return 0;
}