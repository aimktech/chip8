/*
 * cpu.cpp
 * CPU Management Unit implementation
 */

// includes
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "constants.h"
#include "except.h"
#include "cpu.h"

// constants
constexpr int NUM_REGISTERS = 16;
enum Register {
    V0 = 0x00,
    VF = 0x0F
};

// class structure
struct CPU::OpaqueData
{
    MMU *pMMU {nullptr};

    // screen data
    byte_t *pScreen {nullptr};
    word_t screenWidth {0};

    // CPU registers
    byte_t V[NUM_REGISTERS] {};
    word_t I {};
    word_t PC {};
    word_t SP {};

    void create();
    void destroy();
    void reset();

    bool putPixel(int x, int y);
    void clearScreen();
};

// Initialize the structure
void CPU::OpaqueData::create()
{
    // set the screen pointer
    pScreen = pMMU->getPointer(MemoryZone::SCREEN_BEGIN);
    screenWidth = pMMU->readW(MemoryRegister::SCREEN_WIDTH);

    // reset the CPU
    reset();
}

// De-initialize the structure
void CPU::OpaqueData::destroy()
{ }

// Reset the CPU
void CPU::OpaqueData::reset()
{
    I = MemoryZone::ROM_BEGIN;
    PC = MemoryZone::CODE_BEGIN;
    SP = MemoryZone::STACK_END;

    ::memset(&V[0], 0x00, NUM_REGISTERS);
    ::srand(::time(NULL));
}

/* Draw a pixel on the screen
 * Returns:
 *      True if a collision occurred
 */
bool CPU::OpaqueData::putPixel(int x, int y)
{
    int offset = y * screenWidth + x;
    bool isCollision {false};

    if( pScreen[offset] == 1 )
        isCollision = true;

    pScreen[offset] ^= 1;

    return isCollision;
}

// Clear the screen memory
void CPU::OpaqueData::clearScreen()
{
    ::memset(pScreen, 0x00, MemoryZone::SCREEN_SIZE);
}

/* Constructor
 * Args:
 *      pMMU: the pointer to the MMU
 *      pDisplay: the pointer to the Display
 */
CPU::CPU(MMU *pMMU) :
    data_(new (std::nothrow) OpaqueData)
{
    if( data_ == nullptr ) {
        throw CPUError("Unable to allocate CPU data.");
    }

    data_->pMMU = pMMU;
    data_->create();
}

// Destructor
CPU::~CPU()
{
    data_->destroy();
}

// Reset the CPU to it's initial state
void CPU::reset()
{
    data_->reset();
    data_->clearScreen();
}

// Performs a Fetch/Decode/Execute cycle
bool CPU::update()
{
    // read the next instruction
    auto opcode = data_->pMMU->readW(data_->PC);

    // retrieve values from the opcode
    auto addr = (opcode & 0x0FFF);
    auto x = (opcode & 0x0F00) >> 8;
    auto y = (opcode & 0x00F0) >> 4;
    auto value = (opcode & 0x00FF);

    // incrememt PC to next instruction
    data_->PC += 2;

    switch(opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode)
            {
                case 0x00E0:    // CLS
                    data_->clearScreen();
                    break;

                case 0x00EE:    // RET
                    data_->PC = data_->pMMU->readW(data_->SP);
                    data_->SP += 2;
                    break;
            }
            break;

        case 0x1000:    // JP addr
            data_->PC = addr;
            break;

        case 0x2000:    // CALL addr
            data_->SP -= 2;
            data_->pMMU->writeW(data_->SP, data_->PC);
            data_->PC = addr;
            break;

        case 0x3000:    // SE Vx, byte
            if( data_->V[x] ==  value)
                data_->PC += 2;
            break;

        case 0x4000:    // SNE Vx, byte
            if( data_->V[x] !=  value)
                data_->PC += 2;
            break;

        case 0x5000:    // SE Vx, Vy
            if( data_->V[x] == data_->V[y] )
                data_->PC += 2;
            break;

        case 0x6000:    // LD Vx, byte
            data_->V[x] = value;
            break;

        case 0x7000:    // ADD Vx, byte
            data_->V[x] += value;
            break;

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000:  // LD Vx, Vy
                    data_->V[x] = data_->V[y];
                    break;

                case 0x0001:  // OR Vx, Vy
                    data_->V[x] |= data_->V[y];
                    break;

                case 0x0002:  // AND Vx, Vy
                    data_->V[x] &= data_->V[y];
                    break;

                case 0x0003:  // XOR Vx, Vy
                    data_->V[x] ^= data_->V[y];
                    break;

                case 0x0004:  // ADC Vx, Vy
                    {
                        int sum = data_->V[x] + data_->V[y];
                        data_->V[Register::VF] = (sum > 0x00FF) ? 1 : 0;
                        data_->V[x] = (sum & 0xFF);
                    }
                    break;

                case 0x0005:  // SBC Vx, Vy
                    data_->V[Register::VF] = (data_->V[x] > data_->V[y]) ? 1 : 0;
                    data_->V[x] -= data_->V[y];
                    break;

                case 0x0006:  // SHR Vx, 1
                    data_->V[Register::VF] = (data_->V[x] & 0x01);
                    data_->V[x] >>=  1;
                    break;

                case 0x0007:  // SUBN Vx, Vy
                    data_->V[Register::VF] = (data_->V[y] > data_->V[x]) ? 1 : 0;
                    data_->V[x] = data_->V[y] - data_->V[x];
                    break;

                case 0x000E:  // SHL Vx, 1
                    data_->V[Register::VF] = (data_->V[x] & 0x80) ? 1 : 0;
                    data_->V[x] <<= 1;
                    break;
            }
            break;

        case 0x9000:    // SNE Vx, Vy
            if( data_->V[x] != data_->V[y] )
                data_->PC += 2;
            break;

        case 0xA000:    // LD I, addr
            data_->I = addr;
            break;

        case 0xB000:    // JP V0, addr
            data_->PC = addr + data_->V[Register::V0];
            break;

        case 0xC000:    // RND Vx, byte
            data_->V[x] = (::rand() % (0xFF + 1)) & value;
            break;

        case 0xD000:    // DRW Vx, Vy, n
            {
                byte_t height = opcode & 0x000F;

                data_->V[Register::VF] = 0;
                for (int row = 0; row < height; row++)
                {
                    byte_t sprite = data_->pMMU->readB(data_->I + row);
                    for(int col = 0; col < Constants::SPRITE_WIDTH; col++)
                    {
                        // check the upper bit only
                        if( (sprite & 0x80) > 0 )
                        {
                            // if this is true then a collision occured
                            if( data_->putPixel(data_->V[x] + col, data_->V[y] + row) )
                                data_->V[Register::VF] = 1;
                        }

                        // next bit
                        sprite <<= 1;
                    }
                }
            }
            break;

        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E:  // SKP Vx
                    {
                        int key = (int)data_->pMMU->readW(MemoryRegister::KEYBOARD_STATUS);
                        int vx = 1 << data_->V[x];

                        if( (key & vx) == vx )
                            data_->PC += 2;
                    }
                    break;

                case 0x00A1:  // SKNP Vx
                    {
                        int key = (int)data_->pMMU->readW(MemoryRegister::KEYBOARD_STATUS);
                        int vx = 1 << data_->V[x];

                        if( (key & vx) != vx )
                            data_->PC += 2;
                    }
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:  // LD Vx, DT
                    data_->V[x] = data_->pMMU->readB(MemoryRegister::DELAY_TIMER);
                    break;

                case 0x000A:  // LD Vx, K
                    {
                        int key = (data_->pMMU->readW(MemoryRegister::KEYBOARD_STATUS));
                        if( key == 0 )
                            data_->PC -= 2;
                        else
                        {
                            int vx = 1;
                            int mask = vx;
                            while( (key & mask) != mask) {
                                vx += 1;
                                mask = 1 << vx;
                            }

                            data_->V[x] = vx;
                        }
                    }
                    break;

                case 0x0015:  // LD DT, Vx
                    data_->pMMU->writeB(MemoryRegister::DELAY_TIMER, data_->V[x]);
                    break;

                case 0x0018:  // LD ST, Vx
                    data_->pMMU->writeB(MemoryRegister::SOUND_TIMER, data_->V[x]);
                    break;

                case 0x001E:  // ADD I, VX
                    data_->V[Register::VF] = ( (data_->I + data_->V[x]) > 0x0FFF ) ? 1 : 0;
                    data_->I += data_->V[x];
                    break;

                case 0x0029:  // LD F, Vx
                    data_->I = MemoryZone::ROM_BEGIN + (int)(data_->V[x]) * Constants::FONT_SIZE;
                    break;

                case 0x0033:  // LD B, Vx
                    data_->pMMU->writeB(data_->I,     (data_->V[x] / 100));
                    data_->pMMU->writeB(data_->I + 1, (data_->V[x] / 10) % 10);
                    data_->pMMU->writeB(data_->I + 2, (data_->V[x] % 10));
                    break;

                case 0x0055:  // LD [I], Vx
                    switch(x) {
                        case 0x0F: data_->pMMU->writeB(data_->I + 0x0F, data_->V[0x0F]);
                        case 0x0E: data_->pMMU->writeB(data_->I + 0x0E, data_->V[0x0E]);
                        case 0x0D: data_->pMMU->writeB(data_->I + 0x0D, data_->V[0x0D]);
                        case 0x0C: data_->pMMU->writeB(data_->I + 0x0C, data_->V[0x0C]);
                        case 0x0B: data_->pMMU->writeB(data_->I + 0x0B, data_->V[0x0B]);
                        case 0x0A: data_->pMMU->writeB(data_->I + 0x0A, data_->V[0x0A]);
                        case 0x09: data_->pMMU->writeB(data_->I + 0x09, data_->V[0x09]);
                        case 0x08: data_->pMMU->writeB(data_->I + 0x08, data_->V[0x08]);
                        case 0x07: data_->pMMU->writeB(data_->I + 0x07, data_->V[0x07]);
                        case 0x06: data_->pMMU->writeB(data_->I + 0x06, data_->V[0x06]);
                        case 0x05: data_->pMMU->writeB(data_->I + 0x05, data_->V[0x05]);
                        case 0x04: data_->pMMU->writeB(data_->I + 0x04, data_->V[0x04]);
                        case 0x03: data_->pMMU->writeB(data_->I + 0x03, data_->V[0x03]);
                        case 0x02: data_->pMMU->writeB(data_->I + 0x02, data_->V[0x02]);
                        case 0x01: data_->pMMU->writeB(data_->I + 0x01, data_->V[0x01]);
                        case 0x00: data_->pMMU->writeB(data_->I + 0x00, data_->V[0x00]);
                    }

                    data_->I += x + 1;
                    break;

                case 0x0065:  // LD Vx, [I]
                    switch(x) {
                        case 0x0F: data_->V[0x0F] = data_->pMMU->readB(data_->I + 0x0F);
                        case 0x0E: data_->V[0x0E] = data_->pMMU->readB(data_->I + 0x0E);
                        case 0x0D: data_->V[0x0D] = data_->pMMU->readB(data_->I + 0x0D);
                        case 0x0C: data_->V[0x0C] = data_->pMMU->readB(data_->I + 0x0C);
                        case 0x0B: data_->V[0x0B] = data_->pMMU->readB(data_->I + 0x0B);
                        case 0x0A: data_->V[0x0A] = data_->pMMU->readB(data_->I + 0x0A);
                        case 0x09: data_->V[0x09] = data_->pMMU->readB(data_->I + 0x09);
                        case 0x08: data_->V[0x08] = data_->pMMU->readB(data_->I + 0x08);
                        case 0x07: data_->V[0x07] = data_->pMMU->readB(data_->I + 0x07);
                        case 0x06: data_->V[0x06] = data_->pMMU->readB(data_->I + 0x06);
                        case 0x05: data_->V[0x05] = data_->pMMU->readB(data_->I + 0x05);
                        case 0x04: data_->V[0x04] = data_->pMMU->readB(data_->I + 0x04);
                        case 0x03: data_->V[0x03] = data_->pMMU->readB(data_->I + 0x03);
                        case 0x02: data_->V[0x02] = data_->pMMU->readB(data_->I + 0x02);
                        case 0x01: data_->V[0x01] = data_->pMMU->readB(data_->I + 0x01);
                        case 0x00: data_->V[0x00] = data_->pMMU->readB(data_->I + 0x00);
                    }

                    data_->I += x + 1;
                    break;
            }
            break;
    }

    return true;
}
