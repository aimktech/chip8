/*
 * cpu.h
 * CPU Management Unit
 */

// guards
#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

// includes
#include <memory>
#include "types.h"
#include "mmu.h"

// class definition
class CPU
{
    public:     // public methods
        CPU(MMU *pMMU);
        ~CPU();

        // disallow copy/move semantics
        CPU(const CPU&) = delete;
        CPU(CPU&&) = delete;
        CPU& operator=(const CPU&) = delete;
        CPU& operator=(CPU&&) = delete;

        bool update();
        void reset();

    private:    // private members
        struct OpaqueData;
        std::unique_ptr<OpaqueData> data_;
};

#endif  // CHIP8_CPU_H