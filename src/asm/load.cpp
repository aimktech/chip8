/*
 * Load instruction
 */

// includes
#include <list>

#include "asm/comphelpers.h"


// global table of jumps
extern std::list<t_label> jumps_table;

// Load operand
uint16_t load(uint16_t PC, t_token t, Parser* p)
{
    uint16_t value{0};
    uint16_t r1{0}, r2{0};

    // retrieve the two operand
    t_token op1 = p->next();
    t_token op2 = p->next();

    switch(op1.first)
    {
        case TOKEN_REGISTER:
        {
            value = 0;
            r1 = convert(op1.second);

            // 6xkk - LD Vx, byte
            if( op2.first == TOKEN_VALUE ) {
                r2 = convert(op2.second);
                value = 0x6000 | (r1 & 0x0F) << 8 | (r2 & 0xFF);
            }

            // 8xy0 - LD Vx, Vy
            if( op2.first == TOKEN_REGISTER ) {
                r2 = convert(op2.second);
                value = 0x8000 | (r1 & 0x0F) << 8 | (r2 & 0x0F) << 4;
            }

            if( op2.first == TOKEN_OPERAND ) {
                // Fx07 - LD Vx, DT
                if( op2.second.compare("DT") == 0 )
                    value = 0xF007 | (r1 & 0x0F) << 8;

                // Fx0A - LD Vx, K
                if( op2.second.compare("K") == 0 )
                    value = 0xF00A | (r1 & 0x0F) << 8;

                // Fx65 - LD Vx, [I]
                if( op2.second.compare("[I]") == 0 )
                    value = 0xF065 | (r1 & 0x0F) << 8;
            }

            if( value == 0 )
                throw std::string("Invalid second operand for LD.");
        }
        break;

        case TOKEN_OPERAND:
        {
            switch(op2.first)
            {
                case TOKEN_VALUE:   // Annn - LD I, addr
                {
                    if( op1.second.compare("I") != 0 )
                        throw std::string("Invalid first operand for LD.");

                    r2 = convert(op2.second);
                    value = 0xA000 | (r2 & 0x0FFF);
                }
                break;

                case TOKEN_REGISTER:    // a register
                {
                    value = 0;
                    r2 = convert(op2.second);

                    // Fx15 - LD DT, Vx
                    if(op1.second.compare("DT") == 0)
                        value = 0xF015 | (r2 & 0x0F) << 8;

                    // Fx18 - LD ST, Vx
                    if(op1.second.compare("ST") == 0)
                        value = 0xF018 | (r2 & 0x0F) << 8;

                    // Fx29 - LD F, Vx
                    if(op1.second.compare("F") == 0)
                        value = 0xF029 | (r2 & 0x0F) << 8;

                    // Fx33 - LD B, Vx
                    if(op1.second.compare("B") == 0)
                        value = 0xF033 | (r2 & 0x0F) << 8;

                    // Fx55 - LD [I], Vx
                    if(op1.second.compare("[I]") == 0)
                        value = 0xF055 | (r2 & 0x0F) << 8;

                    if( value == 0 )
                        throw std::string("Invalid second operand for LD.");
                }
                break;

                case TOKEN_OPERAND:         // Annn - LD I, label
                {
                    value = 0xA000;
                    // add this to jumps list
                    jumps_table.push_back(std::make_pair(PC, op2.second));
                }
                break;

                default:
                    throw std::string("Invalid second operand for LD.");
            }
        }
        break;

        default:
            throw std::string("Invalid first operand for LD.");
    }

    return value;
}
