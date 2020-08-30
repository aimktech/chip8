/*
 * disassembler.h
 * Chip8 Disassembler main class
 */
#ifndef CHIP8_DISASSEM_H
#define CHIP8_DISASSEM_H

#include <cstdint>
#include <fstream>
#include <set>

// main disassembler class
class Disassembler
{
    public:     // public methods
        Disassembler();                                 // constructor
        ~Disassembler();                                // destructor

        // disable move/copy semantics
        Disassembler(const Disassembler&) = delete;
        Disassembler(Disassembler&&) = delete;
        Disassembler& operator=(const Disassembler&) = delete;
        Disassembler& operator=(Disassembler&&) = delete;

        void loadROM(std::string filename);             // load the ROM in memory
        void discover();                                // discover all the segments of code
        void render();                                  // render the disassembled program

    private:    // private methods
        uint16_t next();                                // return the opcode at the current PC

    private:    // private members
        uint16_t PC_{0};                                // program counter
        uint16_t max_address_{0};                       // maximum address allowed for PC
        std::streampos romsize_{0};                     // size of the ROM loaded
        std::unique_ptr<uint8_t[]> rom_{nullptr};       // the memory where the ROM is loaded
        std::set<uint16_t> labels_;                     // list of labels
        std::set<uint16_t> codemap_;                    // codemap for this ROM

        const int ROM_CODE_BEGIN{0x0200};               // code start address
};

#endif  // CHIP8_DISASSEM_H