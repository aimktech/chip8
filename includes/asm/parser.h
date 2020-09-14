/*
 * parser.h
 * Parse a Chip8 assembly file and create the corresponding flow of tokens
 */
#ifndef CHIP8_ASM_PARSER_H
#define CHIP8_ASM_PARSER_H

#include <iostream>
#include <exception>
#include <fstream>
#include <utility>

#include "types.h"

// maximum size for a token
const int MAX_TOKEN_LENGTH{20};


// exception raised when the parsing is done
class StopIteration: public std::exception
{
    public:
        explicit StopIteration() { }
};

// parse a file and generate tokens
class Parser
{
    public:     // public methods
        Parser(std::string);
        ~Parser();

        t_token next();                     // return the next token

    private:    // private methods
        char get();                         // return the next character in the stream
        char peek();                        // peek the next character in the stream

    private:    // private members
        std::ifstream* is_{nullptr};            // input stream
        char token_[MAX_TOKEN_LENGTH+1]{0};     // current token
        TOKEN_TYPE type_{TOKEN_INIT};           // current token type
        int offset_{0};                         // current character offset in the token string
};

#endif // CHIP8_ASM_PARSER_H