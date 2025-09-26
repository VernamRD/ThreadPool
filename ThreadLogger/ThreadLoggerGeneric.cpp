#include "ThreadLoggerGeneric.h"

#include <iostream>

namespace color
{
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string RESET = "\033[0m";   
}

void ThreadLoggerGeneric::Verbose(const std::string& str)
{
    std::cout << str << '\n';
}

void ThreadLoggerGeneric::Log(const std::string& str)
{
    std::cout << str << '\n';
}

void ThreadLoggerGeneric::Warning(const std::string& str)
{
    std::cout << color::YELLOW << str << color::RESET << '\n';
}
