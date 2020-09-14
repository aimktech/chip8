/*
 * arithm.cpp
 * Arithmetic operations (ADD / SUB)
 */

// includes
#include "asm/comphelpers.h"


// Add operand
uint16_t add(uint16_t PC, t_token t, Parser* p)
{
    uint16_t value{0};

    // retrieve the two operands
    t_token op1 = p->next();
    t_token op2 = p->next();

    if( op1.first == TOKEN_REGISTER )
    {
        int r1 = convert(op1.second);
        int r2 = convert(op2.second);

        if( op2.first == TOKEN_VALUE ) {
            //  7xkk - ADD Vx, byte
            value = (0x7000) | (r1 & 0x0F) << 8 | (r2 & 0xFF);
        }
        if( op2.first == TOKEN_REGISTER ) {
            // 8xy4 - ADD Vx, Vy
            value = (0x8004) | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;
        }

        if( value == 0 )
            throw std::string("Invalid second operand for ADD.");
    }

    if( op1.first == TOKEN_OPERAND )
    {
        // Fx1E - ADD I, Vx
        if( op1.second.compare("I") != 0 )
            throw std::string("Invalid first operand for ADD.");

        if( op2.first != TOKEN_REGISTER )
            throw std::string("Invalid second operand for ADD.");

        int r2 = convert(op2.second);
        value = (0xF01E) | (r2 & 0x0F) << 8;
    }

    return value;
}

// Sub operand
uint16_t sub(uint16_t PC, t_token t, Parser* p)
{
    uint16_t value{0};

    // retrieve the two operands
    t_token op1 = p->next();
    t_token op2 = p->next();

    if( op1.first != TOKEN_REGISTER )
        throw std::string("Invalid first operand for SUB.");

    if( op2.first != TOKEN_REGISTER )
        throw std::string("Invalid second operand for SUB.");

    uint16_t r1 = convert(op1.second);
    uint16_t r2 = convert(op2.second);

    // 8xy5 - SUB Vx, Vy
    if( t.second.compare("SUB") == 0 )
        value = 0x8005 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;

    // 8xy7 - SUBN Vx, Vy
    if( t.second.compare("SUBN") == 0 )
        value = 0x8007 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;

    return value;
}