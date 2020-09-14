/*
 * assembler.h
 * Chip8 Assembler main class
 */
#ifndef CHIP8_ASSEMBLER_H
#define CHIP8_ASSEMBLER_H

// includes
#include <cstdint>

#include "asm/parser.h"

// constants
const int MAX_MEMORY_LENGTH{4096};                      // maximum allowed for code generation
const int ROM_CODE_BEGIN{0x0200};                       // code start address


// main disassembler class
class Assembler
{
    public:     // public methods
        Assembler();                                    // constructor
        ~Assembler();                                   // destructor

        // disable move/copy semantics
        Assembler(const Assembler&) = delete;
        Assembler(Assembler&&) = delete;
        Assembler& operator=(const Assembler&) = delete;
        Assembler& operator=(Assembler&&) = delete;

        bool compile(std::string filename);             // compile a ASM file
        void write(std::string filename);               // write the result to disk

    private:    // private members
        uint16_t PC_{0};                                // program counter
        uint8_t ROM_[MAX_MEMORY_LENGTH]{0};             // where the program is generated
};

#endif  // CHIP8_ASSEMBLER_H