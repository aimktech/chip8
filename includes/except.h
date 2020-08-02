/*
 * except.h
 * Chip8 Exceptions
 */

// guards
#ifndef CHIP8_EXCEPT_H
#define CHIP8_EXCEPT_H

// includes
#include <exception>
#include <string>

// base exception
class BaseExceptError : public std::exception
{
    public:
        explicit BaseExceptError(const char *message) :
            message_(message)
        { }

        virtual const char* what() const throw()
        {
            return message_;
        }

    private:
        const char *message_{nullptr};
};

// exception thrown when an issue with the MMU occures
class MMUError: public BaseExceptError
{
    public:
        explicit MMUError(const char *message) :
            BaseExceptError(message)
        { }
};

// exception thrown when an issue with the CPU occures
class CPUError: public BaseExceptError
{
    public:
        explicit CPUError(const char *message) :
            BaseExceptError(message)
        { }
};

// exception thrown when an issue with the Display occures
class DisplayError: public BaseExceptError
{
    public:
        explicit DisplayError(const char *message) :
            BaseExceptError(message)
        { }
};

// exception thrown when an issue with the VM occures
class VMError: public BaseExceptError
{
    public:
        explicit VMError(const char *message) :
            BaseExceptError(message)
        { }
};

#endif // CHIP8_EXCEPT_H