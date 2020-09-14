/*
 * logic.cpp
 * Logic operations
 */

// includes
#include "asm/comphelpers.h"


// Logic operands
uint16_t logic(uint16_t PC, t_token t, Parser* p)
{
    uint16_t value{0};

    // retrieve the two operand
    t_token op1 = p->next();
    t_token op2 = p->next();

    if( op1.first != TOKEN_REGISTER )
        throw std::string("Invalid first operand.");

    uint16_t r1 = convert(op1.second);
    uint16_t r2 = convert(op2.second);

    if( t.second.compare("OR") == 0 )
        value = 0x8001 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;

    if( t.second.compare("AND") == 0 )
        value = 0x8002 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;

    if( t.second.compare("XOR") == 0 )
        value = 0x8003 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;

    if( t.second.compare("SHR") == 0 )
        value = 0x8006 | (r1 & 0x0F) << 8;

    if( t.second.compare("SHL") == 0 )
        value = 0x800E | (r1 & 0x0F) << 8;

    if(value == 0)
        throw std::string("Invalid logic instruction.");

    return value;
}
