/*
 * vm.h
 * The Chip8 Virtual Machine
 */

// guards
#ifndef CHIP8_VM_H
#define CHIP8_VM_H

// includes
#include <memory>
#include <string>
#include "types.h"

// class definition
class VM
{
    public:
        VM();
        ~VM();

        // disallow copy/move semantics
        VM(const VM&) = delete;
        VM(VM&&) = delete;
        VM& operator=(const VM&) = delete;
        VM& operator=(VM&&) = delete;

        void init();
        void run();
        void shutdown();
        void loadRom(std::string filename);

    private:
        struct OpaqueData;
        std::unique_ptr<OpaqueData> data_;
};

#endif  // CHIP8_VM_H