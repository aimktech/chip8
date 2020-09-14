/*
 * parser.cpp
 * Parse a Chip8 assembly file and create the corresponding flow of tokens
 */

// includes
#include <cstring>

#include "asm/parser.h"

// constructor
Parser::Parser(std::string filename) :
    is_(new (std::nothrow) std::ifstream (filename.c_str(), std::fstream::in))
{
    if( is_ == nullptr )
        throw std::string("Unable to allocate memory for opening the file.");

    if( !is_->is_open() )
        throw std::string("Unable to open the file.");
}

// destructor
Parser::~Parser()
{
    if( is_->is_open() )
        is_->close();
}

// return the next character
char Parser::get()
{
    char c;
    is_->get(c);
    if( is_->eof() )
        throw StopIteration();

    return c;
}

// peek the next character in the file
char Parser::peek()
{
    if( is_->eof() )
        throw StopIteration();

    return is_->peek();
}

// return the next operand
t_token Parser::next()
{
    try
    {
        ::memset(token_, 0, 20);
        offset_ = 0;
        while(true)
        {
            char car = get();

            // remove trailing space at the beginning
            if( (offset_ == 0) && (car == ' ') ) {
                while(car == ' ')
                    car = get();
            }

            // remove comments
            if( car == ';' ) {
                while(car != '\n')
                    car = get();
            }

            // end of line
            if( car == '\n' ) {
                return std::make_pair(TOKEN_EOL, "");
            }

            // detect tokens
            if( offset_ ) {
                // label
                if( car == ':' ) {
                    // remove the rest of the line
                    while(peek() != '\n')
                        get();

                    return std::make_pair(TOKEN_LABEL, token_);
                }

                // operand separator
                if( car == ' ' || car == ',' || peek() == '\n') {
                    // insert the character
                    if(peek() == '\n')
                        token_[offset_] = toupper(car);

                    // token is a value
                    if( token_[0] == '#' ) {
                        return std::make_pair(TOKEN_VALUE, &token_[1]);
                    }

                    // token is a Vx register
                    if( token_[0] == 'V' ) {
                        return std::make_pair(TOKEN_REGISTER, &token_[1]);
                    }

                    if( ::strcmp(token_, "DB") == 0) {
                        return std::make_pair(TOKEN_BYTE, token_);
                    }

                    return std::make_pair(TOKEN_OPERAND, token_);
                }
            }

            // record the character
            token_[offset_++] = toupper(car);
            if( offset_ > MAX_TOKEN_LENGTH )
                throw std::string("The token is too big.");
        }

    } catch(StopIteration) {
        return std::make_pair(TOKEN_END, "");
    }
}