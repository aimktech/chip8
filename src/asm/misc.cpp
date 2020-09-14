/*
 * misc.cpp
 * Miscellaneous operations
 */

// includes
#include "asm/comphelpers.h"

// convert an hexadecimal string value to its integer value
uint16_t convert(std::string value)
{
    // try to convert the string
    try {
        return std::stoi(value, nullptr, 16);
    } catch(std::invalid_argument) {
        throw std::string("Invalid argument type.");
    }
}


// Miscellaneous operands
uint16_t misc(uint16_t PC, t_token t, Parser* p)
{
    uint16_t value{0};
    uint16_t r1{0}, r2{0}, r3{0};

    // 00E0 - CLS
    if(t.second.compare("CLS") == 0)
        return 0x00E0;

    // 00EE - RET
    if(t.second.compare("RET") == 0)
        return 0x00EE;

    // retrieve a first operand
    t_token op1 = p->next();
    r1 = convert(op1.second);

    // Ex9E - SKP Vx
    if( t.second.compare("SKP") == 0 )
        return 0xE09E | (r1 & 0x0F) << 8;

    // ExA1 - SKNP Vx
    if( t.second.compare("SKNP") == 0 )
        return 0xE0A1 | (r1 & 0x0F) << 8;

    // retrieve a second operand
    t_token op2 = p->next();
    r2 = convert(op2.second);

    // Cxkk - RND Vx, byte
    if( t.second.compare("RND") == 0 )
    {
        if( op1.first != TOKEN_REGISTER )
            throw std::string("Invalid first argument in RND operation.");

        if( op2.first != TOKEN_VALUE )
            throw std::string("Invalid second argument in RND operation.");

        return 0xC000 | (r1 & 0x0F) << 8 | (r2 & 0xFF);
    }

    // Dxyn - DRW Vx, Vy, n
    if( t.second.compare("DRW") == 0 )
    {
        // DRW has a third operand
        t_token op3 = p->next();

        if( op1.first != TOKEN_REGISTER )
            throw std::string("Invalid first argument in DRW operation.");

        if( op2.first != TOKEN_REGISTER )
            throw std::string("Invalid second argument in DRW operation.");

        if( op3.first == TOKEN_VALUE )
            r3 = convert(op3.second);
        else
            throw std::string("Invalid third argument in DRW operation.");

        return 0xD000 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4 | r3 & 0x0F;
    }

    throw std::string("Unknown operand.");
}
