
/*
 * mmu.cpp
 * Memory Manager Unit implementation
 */

// includes
#include <cstring>
#include "mmu.h"
#include "constants.h"
#include "except.h"


/* Constructor
 * Args:
 *      size: the size in Bytes of the internal memory
 * Raises:
 *      MMUError in case of error
 */
MMU::MMU() :
    memory_(new (std::nothrow) byte_t [MemoryZone::TOTAL_MEMORY_SIZE])
{
    // memory was not allocated properly
    if( memory_ == nullptr ) {
        throw MMUError("Unable to allocate main memory space.");
    }
    ::memset(memory_.get(), 0, MemoryZone::TOTAL_MEMORY_SIZE);
}

// destructor
MMU::~MMU()
{ }

/* Read a byte (8-bit) in memory
 * Args:
 *      address: the address to read
 * Raises:
 *      MMUError in case of issues
 * Returns:
 *      the byte read from the memory location
 */
byte_t MMU::readB(word_t address) const
{
    byte_t value;

    // address is beyond accessible memory
    if( address >= MemoryZone::UPPER_MEMORY_LIMIT ) {
        throw MMUError("Address outside of memory boundaries.");
    }

    return memory_[address];
}

/* Read a word (16-bit) in memory
 * Args:
 *      address: the address to read
 * Raises:
 *      MMUError in case of issues
 * Returns:
 *      the byte read from the memory location
 */
word_t MMU::readW(word_t address) const
{
    return ((readB(address) << 8) | readB(address + 1));
}

/* Write a byte (8-bit) in memory
 * Args:
 *      address: the address where to write the byte
 *      value: the value to write
 * Raises:
 *      MMUError in case of issues
 */
void MMU::writeB(word_t address, byte_t value)
{
    if( address >= MemoryZone::UPPER_MEMORY_LIMIT ) {
        throw MMUError("Address outside of memory boundaries.");
    }

    // this part is read only
    if( (address >= MemoryZone::ROM_BEGIN) && (address <= MemoryZone::ROM_END)) {
        throw MMUError("Trying to write into Read Only memory.");
    }

    memory_[(int)address] = value;
}

/* Write a word (16-bit) in memory
 * Args:
 *      address: the address where to write the byte
 *      value: the value to write
 * Raises:
 *      MMUError in case of issues
 */
void MMU::writeW(word_t address, word_t value)
{
    // MSB first
    writeB(address, (value & 0xFF00) >> 8);
    writeB(address+1, (value & 0xFF));
}

/* Load data in memory
 * Args:
 *      address: the target address in memory
 *      size: the size of the data
 *      buffer: the source buffer to load the data from
 * Raises:
 *      MMUError in case of issues
 */
void MMU::loadMemory(word_t address, word_t size, byte_t *buffer)
{
    if( address + size > MemoryZone::UPPER_MEMORY_LIMIT ) {
        throw MMUError("Address outside of memory boundaries.");
    }

    ::memcpy(&memory_[address], buffer, size);
}

/* Return a pointer from a memory zone
 * Args:
 *      address: the requested memory address
 * Raises:
 *      MMUError in case of error
 * Returns:
 *      the byte* pointer to the memory address
 */
byte_t* MMU::getPointer(word_t address)
{
    if( address >= MemoryZone::UPPER_MEMORY_LIMIT ) {
        throw MMUError("Address outside of memory boundaries.");
    }

    return &memory_[address];
}