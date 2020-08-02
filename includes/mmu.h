/*
 * mmu.h
 * Memory Management Unit
 */

// guards
#ifndef CHIP8_MMU_H
#define CHIP8_MMU_H

// includes
#include <memory>
#include "types.h"

// class definition
class MMU
{
    public:     // public methods
        MMU();
        ~MMU();

        // disallow copy/move semantics
        MMU(const MMU&) = delete;
        MMU(MMU&&) = delete;
        MMU& operator=(const MMU&) = delete;
        MMU& operator=(MMU&&) = delete;

        byte_t readB(word_t address) const;
        word_t readW(word_t address) const;

        void writeB(word_t address, byte_t value);
        void writeW(word_t address, word_t value);

        void loadMemory(word_t address, word_t size, byte_t *buffer);

        byte_t* getPointer(word_t address);

    private:    // private members
        std::unique_ptr<byte_t[]> memory_;
};


#endif // CHIP8_MMU_H