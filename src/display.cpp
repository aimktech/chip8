/*
 * display.cpp
 * Display Management Unit implementation
 */

// includes
#include <SDL2/SDL.h>
#include "display.h"
#include "except.h"
#include "constants.h"

// Display structure
struct Display::OpaqueData
{
    MMU *pMMU;

    SDL_Window *pWindow{nullptr};
    SDL_Renderer *pRenderer{nullptr};

    void create();
    void destroy();
};

/* Initialize the SDL library
 * Raises:
 *      DisplayError in case of issues
 */
void Display::OpaqueData::create()
{
    // read the screen parameters from the memory
    int width  = pMMU->readW(MemoryRegister::SCREEN_WIDTH) * pMMU->readB(MemoryRegister::SCREEN_XSCALE);
    int height = pMMU->readW(MemoryRegister::SCREEN_HEIGHT) * pMMU->readB(MemoryRegister::SCREEN_YSCALE);

    // create SDL window
    pWindow = SDL_CreateWindow("Chip-8 Emulator",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                width, height,
                                SDL_WINDOW_SHOWN);
    if( pWindow == nullptr ) {
        throw DisplayError(SDL_GetError());
    }

    // create SDL render
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);
    if( pRenderer == nullptr ) {
        throw DisplayError(SDL_GetError());
    }
}

// Cleanup the SDL library
void Display::OpaqueData::destroy()
{
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
}

/* Constructor
 * Args:
 *      pMMU: the pointer to the MMU
 *      width: the width of the screen
 *      height: the height of the screen
 *      xscale: the scale on the X-axis
 *      yscale: the scale on the Y-axis
 * Raises:
 *      DisplayError in case of issues
 */
Display::Display(MMU *pMMU, int width, int height, int xscale, int yscale) :
    data_(new (std::nothrow) OpaqueData)
{
    if ( data_ == nullptr ) {
        throw DisplayError("Cannot allocate memory for the Display structure.");
    }
    data_->pMMU = pMMU;

    // set the corresponding memory registers
    pMMU->writeW(MemoryRegister::SCREEN_WIDTH,  width);
    pMMU->writeW(MemoryRegister::SCREEN_HEIGHT, height);
    pMMU->writeB(MemoryRegister::SCREEN_XSCALE, xscale);
    pMMU->writeB(MemoryRegister::SCREEN_YSCALE, yscale);

    // initialize the display
    data_->create();
}

// Destructor
Display::~Display()
{
    data_->destroy();
}

// render the memory buffer onto the screen
void Display::render()
{
    // get a pointer to the screen data in memory
    byte_t *ptr = data_->pMMU->getPointer(MemoryZone::SCREEN_BEGIN);

    // retrieve the values from memory
    int width  = data_->pMMU->readW(MemoryRegister::SCREEN_WIDTH);
    int height = data_->pMMU->readW(MemoryRegister::SCREEN_HEIGHT);
    int xscale = data_->pMMU->readB(MemoryRegister::SCREEN_XSCALE);
    int yscale = data_->pMMU->readB(MemoryRegister::SCREEN_YSCALE);

    // set the SDL Rect structure
    SDL_Rect r;
    r.w = xscale;
    r.h = yscale;

    // reset the screen
    SDL_SetRenderDrawColor(data_->pRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(data_->pRenderer);

    // set the drawing color
    SDL_SetRenderDrawColor(data_->pRenderer, 0xff, 0xff, 0xff, 0xff);

    // render the screen
    for( int y = 0; y < height; y++)
    {
        int offset = y * width;
        r.y = y * yscale;
        for(int x = 0; x < width; x++)
        {
            if( ptr[offset + x] ) {
                r.x = x * xscale;
                SDL_RenderFillRect(data_->pRenderer, &r);
            }
        }
    }

    SDL_RenderPresent(data_->pRenderer);
}
