/*
 * disassembler.cpp
 * Chip8 Disassembler main class
 */

#include <disassembler.h>
#include <iostream>
#include <string>
#include <queue>

/*
 * helper functions
 */

// string formatting in C++11 as explained in Stack Overflow post
// https://stackoverflow.com/a/26221725
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

// opcode to string transformation
// return the string representation of an opcode
std::string toString(uint16_t opcode)
{
    // decode the opcode
    auto addr = (opcode & 0x0fff);
    auto x = (opcode & 0x0f00) >> 8;
    auto y = (opcode & 0x00f0) >> 4;
    auto value = (opcode & 0x00ff);
    auto group = (opcode & 0xF000);

    // retrieve the instruction from the opcode value
    switch(group)
    {
        case 0x0000:
            if( opcode == 0x00E0)
                return std::string("CLS");

            if( opcode == 0x00EE)
                return std::string("RET");

            break;

        case 0x1000:    // JP addr
            return string_format("JP   L%003X", addr);

        case 0x2000:    // CALL addr
            return string_format("CALL L%003X", addr);

        case 0x3000:    // SE Vx, value
            return string_format("SE   V%X, #%02X", x, value);

        case 0x4000:    // SNE Vx, value
            return string_format("SNE  V%X, #%02X", x, value);

        case 0x5000:    // SE Vx, Vy
            return string_format("SE   V%X, V%X", x, y);

        case 0x6000:    // LD Vx, value
            return string_format("LD   V%X, #%02X", x, value);

        case 0x7000:    // ADD Vx, value
            return string_format("ADD  V%X, #%02X", x, value);

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000:    // LD Vx, Vy
                    return string_format("LD   V%X, V%X", x, y);
                case 0x0001:    // OR Vx, Vy
                    return string_format("OR   V%X, V%X", x, y);
                case 0x0002:    // AND Vx, Vy
                    return string_format("AND  V%X, V%X", x, y);
                case 0x0003:    // XOR Vx, Vy
                    return string_format("XOR  V%X, V%X", x, y);
                case 0x0004:    // ADC Vx, Vy
                    return string_format("ADD  V%X, V%X", x, y);
                case 0x0005:    // SBC Vx, Vy
                    return string_format("SUB  V%X, V%X", x, y);
                case 0x0006:    // SHR Vx, 1
                    return string_format("SHR  V%X, #1", x);
                case 0x0007:    // SUBN Vx, Vy
                    return string_format("SUBN V%X, V%X", x, y);
                case 0x000E:    // SHL Vx, 1
                    return string_format("SHL  V%X, #1", x);
            }
            break;

        case 0x9000:    // SNE Vx, Vy
            return string_format("SNE  V%X, V%X", x, y);

        case 0xA000:    // LD I, addr
            return string_format("LD   I, L%003X", addr);

        case 0xB000:    // JP V0, addr
            return string_format("JP   V0, L%003X", addr);

        case 0xC000:    // RND Vx, value
            return string_format("RND  V%X, #%02X", x, value);

        case 0xD000:    // DRW Vx, Vy, n
            return string_format("DRW  V%X, V%X, #%02X", x, y, value & 0x000F);

        case 0xE000:
            // SKP Vx
            if( (opcode & 0x00FF) == 0x009E)
                return string_format("SKP  V%X", x);

            // SKNP Vx
            if( (opcode & 0x00FF) == 0x00A1)
                return string_format("SKNP V%X", x);
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:    // LD Vx, DT
                    return string_format("LD   V%X, DT", x);
                case 0x000A:    // LD Vx, K
                    return string_format("LD   V%X, K", x);
                case 0x0015:    // LD DT, Vx
                    return string_format("LD   DT, V%X", x);
                case 0x0018:    // LD ST, Vx
                    return string_format("LD   ST, V%X", x);
                case 0x001E:    // ADD I, Vx
                    return string_format("ADD  I, V%X", x);
                case 0x0029:    // LD F, Vx
                    return string_format("LD   F, V%X", x);
                case 0x0033:    // LD B, Vx
                    return string_format("LD   B, V%X", x);
                case 0x0055:    // LD [I], Vx
                    return string_format("LD   [I], V%X", x);
                case 0x0065:    // LD Vx, [I]
                    return string_format("LD   V%X, [I]", x);
            }
            break;
    }

    return string_format("(%0004X)", opcode);
}

/*
 * class methods
 */

// constructor
Disassembler::Disassembler()
{ }

// destructor
Disassembler::~Disassembler()
{ }

// return the next opcode pointed by the PC
uint16_t Disassembler::next( )
{
    // don't forget to remove the offset
    return ((rom_[PC_ - ROM_CODE_BEGIN] << 8) | rom_[PC_ - ROM_CODE_BEGIN + 1]);
}

// load the ROM
void Disassembler::loadROM(std::string filename)
{
    // check if we can access the file
    std::ifstream romfile (filename, std::ios::in | std::ios::binary | std::ios::ate );
    if( !romfile.is_open() ) {
        throw std::string("Unable to load the ROM.");
    }

    // retrieve the size of the ROM
    romsize_ = romfile.tellg();
    max_address_ = ROM_CODE_BEGIN + romsize_;

    // print the file information
    std::cout << ";----------------------------------------------------" << std::endl;
    std::cout << "; ROM Name: " << filename << std::endl;
    std::cout << "; ROM Size: " << romsize_ << " Bytes" << std::endl;
    std::cout << ";----------------------------------------------------" << std::endl;

    // allocate the memory
    rom_.reset(new (std::nothrow) uint8_t [romsize_]);
    if( rom_ == nullptr )
        throw std::string("Unable to allocate sufficient memory for the ROM.");

    // read the file
    romfile.seekg(0, std::ios::beg);
    auto block = 0;
    while( block < romsize_ ) {
        rom_[block++] = (uint8_t)romfile.get();
    }
    romfile.close();
}

// discover the code
void Disassembler::discover()
{
    // queue to register all the code segments to identify
    std::queue<uint16_t> segments;

    // push the code begin segment in the queue
    segments.push(ROM_CODE_BEGIN);

    // mainloop
    while( !segments.empty() )
    {
        // retrieve the next element from the queue
        PC_ = segments.front();
        segments.pop();

        // walk through this segment of code
        while( (PC_ < max_address_) && (codemap_.find(PC_) == codemap_.end()))
        {
            // retrieve the next opcode
            uint16_t opcode = next();

            // mark this address as code
            codemap_.insert(PC_);

            // move the PC forward
            PC_ += 2;

            /*-------------------
             * analyze the opcode
             *-------------------*/

            // this is a RET so we stop here
            if( opcode == 0x00EE )
                break;

            // these opcodes move the PC_ and branch out the code to a different segment
            switch(opcode & 0xF000)
            {
                case 0x1000:    // JUMP
                    PC_ = opcode & 0x0FFF;      // unconditional jump
                    labels_.insert(PC_);        // record the destination label
                    break;

                case 0x2000:    // CALL
                    segments.push(PC_);         // add a new segment to check
                    PC_ = opcode & 0x0FFF;
                    labels_.insert(PC_);        // record the destination label
                    break;

                case 0x3000:    // SE
                case 0x4000:    // SNE
                case 0x5000:    // SE
                case 0x9000:    // SNE
                    segments.push(PC_ + 2);     // add a new segment to check
                    break;

                case 0xE000:    // SKP & SKNP
                    switch(opcode & 0xF0FF)
                    {
                        case 0xE09E:    // SKP
                        case 0xE0A1:    // SKNP
                            segments.push(PC_ + 2);
                            break;
                    }
                    break;

                // this is to record the address for the Index register
                case 0xA000:    // LD I, addr
                    labels_.insert(opcode & 0x0FFF);
                    break;

                // this opcode is very problematic for the disassembler
                // so we just fail here.
                case 0xB000:
                    throw std::string("Encounter instruction 'JP V0, addr'. Unable to disassemble the code.");
            }
        }
    }
}

// render the code
void Disassembler::render()
{
    int count{0};
    PC_ = ROM_CODE_BEGIN;

    // loop through the whole ROM
    while( PC_ < max_address_ )
    {
        // check if this address is in the labels list
        if( labels_.find(PC_) != labels_.end() )
        {
            // print the label on a separate line
            if( count ) {
                std::cout << std::endl;
                count = 0;
            }
            std::cout << string_format("L%003X:", PC_) << std::endl;
        }

        // process data block
        if( codemap_.find(PC_) == codemap_.end() )
        {

            // format the data
            if( count == 0 )
                std::cout << string_format("    DB   #%02X", rom_[PC_ - ROM_CODE_BEGIN] & 0xFF);
            else
                std::cout << string_format(", #%02X", rom_[PC_ - ROM_CODE_BEGIN] & 0xFF);

            // print only 4 bytes per line
            count += 1;
            if( count % 4 == 0 ) {
                std::cout << std::endl;
                count = 0;
            }

            // for data, we move the PC to the next byte
            PC_ += 1;
            continue;
        }

        // if we previously have some data printed,
        // insert a new line
        if( count ) {
            std::cout << std::endl;
            count = 0;
        }

        // print the opcode
        uint16_t opcode = next();
        std::cout << string_format("    %-30s; %0004X", toString(opcode).c_str(), opcode) << std::endl;

        // move PC to the next opcode
        PC_ += 2;
    }

    // insert an extra line
    std::cout << std::endl;
}
