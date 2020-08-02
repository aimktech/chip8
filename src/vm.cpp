/*
 * vm.cpp
 * Chip8 Virtual Machine implementation
 */

// includes
#include <fstream>
#include <SDL2/SDL.h>

#include "vm.h"
#include "mmu.h"
#include "cpu.h"
#include "display.h"
#include "keyboard.h"
#include "romset.h"
#include "constants.h"
#include "except.h"

// Virtual Machine structure
struct VM::OpaqueData
{
    std::unique_ptr<MMU> memory;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<Display> display;
    std::unique_ptr<Keyboard> keyboard;

    void create();
    void destroy();
    void initMemory();
    void updateTimers();
};

// initialize structure
void VM::OpaqueData::create()
{
    // create main memory management unit
    memory = std::unique_ptr<MMU>(new (std::nothrow) MMU);
    if( memory == nullptr )
        throw VMError("Unable to allocate memory for the Memory Unit.");


    // create display
    display = std::unique_ptr<Display>(new (std::nothrow) Display(
                                    memory.get(),
                                    MemoryDefaultValue::SCREEN_WIDTH, MemoryDefaultValue::SCREEN_HEIGHT,
                                    MemoryDefaultValue::SCREEN_XSCALE, MemoryDefaultValue::SCREEN_YSCALE));
    if( display == nullptr )
        throw VMError("Unable to allocate memory for the Display Unit.");

    // create the keyboard
    keyboard = std::unique_ptr<Keyboard>(new (std::nothrow) Keyboard(memory.get()));
    if( keyboard == nullptr )
        throw VMError("Unable to allocate memory for the Keyboard Unit.");


    // create CPU
    cpu = std::unique_ptr<CPU>(new (std::nothrow) CPU(memory.get()));
    if( cpu == nullptr )
        throw VMError("Unable to allocate memory for the CPU Unit.");

    // init the memory
    initMemory();
}

// de-initialize structure
void VM::OpaqueData::destroy()
{ }

// initialize VM memory
void VM::OpaqueData::initMemory()
{
    // initialize the romset
    memory->loadMemory(MemoryZone::ROM_BEGIN, sizeof(romset), &romset[0]);
}

// update timers
void VM::OpaqueData::updateTimers()
{
    byte_t value{0};

    // update sound timer
    value = memory->readB(MemoryRegister::SOUND_TIMER);
    if(value > 0) {
        value--;
        memory->writeB(MemoryRegister::SOUND_TIMER, value);
    }

    // update delay timer
    value = memory->readB(MemoryRegister::DELAY_TIMER);
    if(value > 0) {
        value--;
        memory->writeB(MemoryRegister::DELAY_TIMER, value);
    }
}

// Constructor
VM::VM() :
    data_(new (std::nothrow) OpaqueData)
{
    if( data_ == nullptr ) {
        throw VMError("Unable to allocate memory for VM structure.");
    }

    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {
        throw VMError("Unable to initialize SDL library.");
    }

    data_->create();
}

// desctructor
VM::~VM()
{
    data_->destroy();
    SDL_Quit();
}

// VM initialization
void VM::init()
{
    // nothing to be done here
}

// VM mainloop
void VM::run()
{
    bool quit = false;
    SDL_Event e;

    int fps = 60;
    int framerate = 1000 / fps;
    int lasttime = SDL_GetTicks();
    int speed = 1;

    while(!quit)
    {
        // treats all events
        if( SDL_PollEvent(&e) != 0 ) {
            if( e.type == SDL_QUIT )
                quit = true;
            else
            {
                if( e.type == SDL_KEYDOWN )
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:       // Quit the emulator with ESC
                            quit = true;
                            break;

                        case SDLK_F3:           // F3 to increase the emulator speed
                            speed += 1;
                            if( speed > 20 )
                                speed = 20;
                            break;
                        case SDLK_F2:           // F2 to reset the speed to 1
                            speed = 1;
                            break;
                        case SDLK_F1:           // F1 to decrease the emulator speed
                            speed -= 1;
                            if( speed <= 1 )
                                speed = 1;
                            break;

                        case SDLK_F10:          // Reset the emulator
                            speed = 1;
                            data_->cpu->reset();
                            break;

                        default:
                            data_->keyboard->update(e);
                    }
                }

                if( e.type == SDL_KEYUP )
                    data_->keyboard->update(e);
            }
        }

        // update CPU
        for(int i =0; i<speed; i++) {
            data_->cpu->update();
        }

        // update display
        data_->display->render();

        // update timers at 60fps
        if( SDL_GetTicks() - lasttime >= framerate ) {

            // update timers
            data_->updateTimers();

            // update time variable
            lasttime = SDL_GetTicks();
        }
    }
}

// VM shutdown
void VM::shutdown()
{ }

/* Load a ROM inside the VM memory
 * Args:
 *      filename: the path to the ROM
 * Raises:
 *      VMError in case of issues
 */
void VM::loadRom(std::string filename)
{
    std::ifstream romfile (filename, std::ios::in | std::ios::binary | std::ios::ate );

    if( !romfile.is_open() ) {
        throw VMError("Unable to load the ROM.");
    }

    // retrieve the size of the ROM
    std::streampos size = romfile.tellg();
    char* memblock = new char[size];

    // read the file
    romfile.seekg(0, std::ios::beg);
    romfile.read(memblock, size);
    romfile.close();

    // load the code in memory
    data_->memory->loadMemory(MemoryZone::CODE_BEGIN, size, reinterpret_cast<byte_t*>(memblock));
    delete[] memblock;
}
