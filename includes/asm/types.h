/* types.h
 * Types and Enums for the Chip8 assembler
 */

#ifndef CHIP8_ASM_TYPES_H
#define CHIP8_ASM_TYPES_H

// includes
#include <string>
#include <utility>      // std::pair


// Token types for the parser
enum TOKEN_TYPE
{
    TOKEN_INIT,         // initialization token
    TOKEN_OPERAND,      // an operand
    TOKEN_VALUE,        // a value
    TOKEN_LABEL,        // a label (xxx:)
    TOKEN_REGISTER,     // a register (V[0-F])
    TOKEN_BYTE,         // a data byte
    TOKEN_EOL,          // end of line
    TOKEN_END,          // end of file
};

// a token is a type and a string
typedef std::pair<TOKEN_TYPE, std::string> t_token;

// definition of a label in the code
typedef std::pair<uint16_t, std::string> t_label;

#endif // CHIP8_ASM_TYPES_H