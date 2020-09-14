/*
 * assembler.cpp
 * Chip8 Assembler main class
 */

// includes
#include <iostream>
#include <cstdint>
#include <cstring>      // ::memset
#include <list>         // std::list
#include <map>          // std::map


#include "assembler.h"

#include "asm/types.h"
#include "asm/parser.h"
#include "asm/comphelpers.h"


// table to hold all the jumps
std::list<t_label> jumps_table;

// mapping table between the labels name and their location in the code
std::map<std::string, uint16_t> labels_map;

// opcodes/byte codes functions mapping table
typedef uint16_t (*callback)(uint16_t, t_token, Parser*);
std::map<std::string, callback> opcodes_map;

extern uint16_t convert(std::string);


// constructor
Assembler::Assembler()
{
    // prepare the mapping table
    opcodes_map["JP"]   = &jumpOrCall;
    opcodes_map["CALL"] = &jumpOrCall;

    opcodes_map["LD"]   = &load;

    opcodes_map["SE"]   = &skip;
    opcodes_map["SNE"]  = &skip;

    opcodes_map["CLS"]  = &misc;
    opcodes_map["RET"]  = &misc;
    opcodes_map["SKP"]  = &misc;
    opcodes_map["SKNP"] = &misc;
    opcodes_map["RND"]  = &misc;
    opcodes_map["DRW"]  = &misc;

    opcodes_map["AND"]  = &logic;
    opcodes_map["OR"]   = &logic;
    opcodes_map["XOR"]  = &logic;
    opcodes_map["SHR"]  = &logic;
    opcodes_map["SHL"]  = &logic;

    opcodes_map["ADD"]  = &add;
    opcodes_map["SUB"]  = &sub;
    opcodes_map["SUBN"] = &sub;
}

// destructor
Assembler::~Assembler()
{
}


// compile an ASM file
bool Assembler::compile(std::string filename)
{
    uint16_t lines{1};  // parsed lines

    // create a new parser
    Parser* p = new (std::nothrow) Parser(filename);
    if( p == nullptr )
        throw std::string("Unable to allocate memory for the parser.");

    // token & output buffer
    t_token t{TOKEN_INIT, ""};
    ::memset(ROM_, 0, MAX_MEMORY_LENGTH);

    // compile the source
    try
    {
        while(true)
        {
            // retrieve the next token
            t = p->next();

            // increment the counter on end-of-line
            if( t.first == TOKEN_EOL ) {
                lines++;
            }

            // token is a label
            if( t.first == TOKEN_LABEL ) {
                labels_map[t.second] = PC_;
            }

            // token is an operand
            if( t.first == TOKEN_OPERAND ) {
                // lookup for the operand in the map
                auto it = opcodes_map.find(t.second);

                // found it
                if( it != opcodes_map.end()) {
                    // call the function
                    uint16_t value = (it->second)(PC_, t, p);

                    // record the result in the memory buffer
                    ROM_[PC_++] = (value & 0xFF00) >> 8;
                    ROM_[PC_++] = (value & 0x00FF);
                }
                else {
                    throw std::string("Unknow instruction.");
                }
            }

            // token is a series of bytes
            if( t.first == TOKEN_BYTE ) {
                // retrieve all the values associated with "DATA BYTE"
                t_token x = p->next();
                while( (x.first != TOKEN_EOL) && (x.first != TOKEN_END)) {
                    uint8_t value = convert(x.second) & 0xFF;
                    ROM_[PC_++] = value;
                    x = p->next();
                }

                // keep the lines counter accurate
                lines++;
            }

            // end of the file
            if (t.first == TOKEN_END)
                break;

            // check if we reached the maximum allowed memory
            if( PC_ >= MAX_MEMORY_LENGTH )
                throw std::string("Maximum allowed memory reached.");
        }
    } catch(std::string& e) {
        std::cout << "An error occured during compilation at line " << lines << " :" << std::endl;
        std::cout << e << std::endl;
        delete p;
        return false;
    }

    // print the number of lines parsed
    std::cout << (lines-1) << " lines parsed." << std::endl;

    // resolve the jumps
    for(auto jt = jumps_table.begin(); jt != jumps_table.end(); ++jt)
    {
        uint16_t offset{jt->first};
        std::string label{jt->second};

        // look for the target address in the map
        auto lm = labels_map.find(label);

        if( lm != labels_map.end() ) {
            // Move the address according to the beginning of the ROM address
            uint16_t value = lm->second + ROM_CODE_BEGIN;

            // replace the placeholders
            ROM_[offset] = ROM_[offset] | (value & 0x0F00) >> 8;
            ROM_[offset+1] = value & 0xFF;
        } else {
            std::cout << "Cannot find label " << label << " in the source." << std::endl;
            return false;
        }
    }

    // release parser memory
    delete p;

    return true;
}

// write the compiled code to disk
void Assembler::write(std::string filename)
{
    std::ofstream fh(filename.c_str(), std::ios::out|std::ios::binary);
    if( fh.is_open() )
    {
        for(int i=0; i < PC_; i++) {
            fh << ROM_[i];
        }
    }
    fh.close();
    std::cout << PC_ << " bytes generated." << std::endl;
}