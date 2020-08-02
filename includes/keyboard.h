/*
 * keyboard.h
 * Keyboard management unit
 */

// guards
#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

// includes
#include <SDL2/SDL.h>
#include "types.h"
#include "mmu.h"

// class definition
class Keyboard
{
    public:
        Keyboard(MMU *pMMU);
        ~Keyboard();

        // disallow copy/move semantics
        Keyboard(const Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(Keyboard&&) = delete;

        // update keyboard
        void update(SDL_Event &e);

    private:
        MMU *pMMU_;
};

#endif // CHIP8_KEYBOARD_H