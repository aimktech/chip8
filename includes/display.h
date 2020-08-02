/*
 * display.h
 * Display Management Unit
 */

// guards
#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

// includes
#include <memory>
#include "types.h"
#include "mmu.h"

// class definition
class Display
{
    public:
        Display(MMU *pMMU, int width, int height, int xscale, int yscale);
        ~Display();

        // disallow copy/move semantics
        Display(const Display&) = delete;
        Display(Display&&) = delete;
        Display& operator=(const Display&) = delete;
        Display& operator=(Display&&) = delete;

        // render the memory buffer onto the screen
        void render();

    private:
        struct OpaqueData;
        std::unique_ptr<OpaqueData> data_;
};

#endif  // CHIP8_DISPLAY_H
