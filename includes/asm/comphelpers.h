/*
 * comphelpers.h
 * Functions to transform tokens into bytes code
 */

#ifndef CHIP8_ASM_COMPHELPERS_H
#define CHIP8_ASM_COMPHELPERS_H

#include <string>       // std::stoi

#include "types.h"
#include "parser.h"

// convert an hexadecimal string value to its integer value
uint16_t convert(std::string value);


// Jump or Call operands
uint16_t jumpOrCall(uint16_t, t_token, Parser*);

// Load operand
uint16_t load(uint16_t, t_token, Parser*);

// Add operand
uint16_t add(uint16_t, t_token, Parser*);

// Sub operand
uint16_t sub(uint16_t, t_token, Parser*);

// Logic operands
uint16_t logic(uint16_t, t_token, Parser*);

// Skip operands
uint16_t skip(uint16_t, t_token, Parser*);

// Miscellaneous operands
uint16_t misc(uint16_t, t_token, Parser*);


#endif // CHIP8_ASM_COMPHELPERS_H