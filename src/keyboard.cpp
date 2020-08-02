/*
 * keyboard.cpp
 * Keyboard management unit implementation
 */

// includes
#include <iostream>
#include <map>
#include "keyboard.h"
#include "constants.h"

// global key mapping
std::map<int, int> keymap;

/* Constructor
 * Args:
 *      pMMU: the pointer to the MMU
 */
Keyboard::Keyboard(MMU *pMMU) :
    pMMU_(pMMU)
{
    // map each SDL key to the Chip8 keyboard
    keymap[120] = 0x00;     // x
    keymap[49]  = 0x01;     // 1
    keymap[50]  = 0x02;     // 2
    keymap[51]  = 0x03;     // 3
    keymap[113] = 0x04;     // q
    keymap[119] = 0x05;     // w
    keymap[101] = 0x06;     // e
    keymap[97]  = 0x07;     // a
    keymap[115] = 0x08;     // s
    keymap[100] = 0x09;     // d
    keymap[122] = 0x0a;     // z
    keymap[99]  = 0x0b;     // c
    keymap[52]  = 0x0c;     // 4
    keymap[114] = 0x0d;     // r
    keymap[102] = 0x0e;     // f
    keymap[118] = 0x0f;     // v

    // initialize the keyboard status
    pMMU->writeW(MemoryRegister::KEYBOARD_STATUS, 0x0000);
}

// Destructor
Keyboard::~Keyboard()
{ }

/* Update the keyboard status
 * Args:
 *      e: the SDL event
 */
void Keyboard::update(SDL_Event &e)
{
    // only the keyboard event
    if( (e.type != SDL_KEYDOWN) && (e.type != SDL_KEYUP) )
        return;

    // look for the key in the mapping
    auto it = keymap.find((int)e.key.keysym.sym);

    if ( it != keymap.end() )
    {
        // compute the mask to set the corresponding bit
        word_t value = 1 << keymap[(int)e.key.keysym.sym];
        word_t status = pMMU_->readW(MemoryRegister::KEYBOARD_STATUS);

        if( e.type == SDL_KEYDOWN ) {
            status = status | value;
        }
        if( e.type == SDL_KEYUP ) {
            status = status & ~value;
        }

        pMMU_->writeW(MemoryRegister::KEYBOARD_STATUS, status);
    }
}
