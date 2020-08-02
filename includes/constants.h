/*
 * constants.h
 */
// guards
#ifndef CHIP8_CONSTANTS_H
#define CHIP8_CONSTANTS_H

// includes
#include "types.h"

// define memory zones
namespace MemoryZone
{
    // ROM
    inline constexpr int ROM_SIZE  { 512 };
    inline constexpr int ROM_BEGIN { 0x0000 };
    inline constexpr int ROM_END   { ROM_BEGIN + ROM_SIZE - 1 };

    // CODE + ROM should be 4,096 bytes
    inline constexpr int CODE_SIZE  { 3584 };
    inline constexpr int CODE_BEGIN { ROM_END + 1 };
    inline constexpr int CODE_END   { CODE_BEGIN + CODE_SIZE - 1 };

    // Stack
    inline constexpr int STACK_SIZE  { 32 * 16 };
    inline constexpr int STACK_BEGIN { CODE_END + 1 };
    inline constexpr int STACK_END   { STACK_BEGIN + STACK_SIZE - 1 };

    // Peripherals
    inline constexpr int PERIPH_SIZE  { 32 };
    inline constexpr int PERIPH_BEGIN { STACK_END + 1 };
    inline constexpr int PERIPH_END   { PERIPH_BEGIN + PERIPH_SIZE - 1 };

    // Screen
    inline constexpr int SCREEN_SIZE  { 2048 };
    inline constexpr int SCREEN_BEGIN { PERIPH_END + 1 };
    inline constexpr int SCREEN_END   { SCREEN_BEGIN + SCREEN_SIZE - 1 };

    // Upper limit of memory
    inline constexpr int UPPER_MEMORY_LIMIT { SCREEN_END + 1 };

    // Total size of memory
    inline constexpr int TOTAL_MEMORY_SIZE = ROM_SIZE \
                                + CODE_SIZE \
                                + STACK_SIZE \
                                + PERIPH_SIZE \
                                + SCREEN_SIZE;
};

// define specific memory registers
namespace MemoryRegister
{
    /* keyboard status (16-bit)
     * each bit corresponds to one of the keyboard key
     * set to 1 when the key is pressed
     * set to 0 otherwise
     */
    inline constexpr int KEYBOARD_STATUS { MemoryZone::PERIPH_BEGIN };

    // delay timer (8-bit)
    inline constexpr int DELAY_TIMER     { MemoryZone::PERIPH_BEGIN + 2 };

    // sound timer (8-bit)
    inline constexpr int SOUND_TIMER     { MemoryZone::PERIPH_BEGIN + 4 };

    // screen width (16-bit)
    inline constexpr int SCREEN_WIDTH    { MemoryZone::PERIPH_BEGIN + 6 };

    // screen height (16-bit)
    inline constexpr int SCREEN_HEIGHT   { MemoryZone::PERIPH_BEGIN + 8 };

    // screen xscale (8-bit)
    inline constexpr int SCREEN_XSCALE   { MemoryZone::PERIPH_BEGIN + 10 };

    // screen yscale (8-bit)
    inline constexpr int SCREEN_YSCALE   { MemoryZone::PERIPH_BEGIN + 12 };
} ;

// default values
namespace MemoryDefaultValue
{
    // screen resolution
    inline constexpr int SCREEN_WIDTH  { 64 };
    inline constexpr int SCREEN_HEIGHT { 32 };

    // screen scale
    inline constexpr int SCREEN_XSCALE { 8 };
    inline constexpr int SCREEN_YSCALE { 8 };
};

// other constants
namespace Constants
{
    inline constexpr int SPRITE_WIDTH { 8 };
    inline constexpr int FONT_SIZE { 5 };
};

#endif // CHIP8_CONSTANTS_H
