#include "ThreadLoggerGeneric.h"

#include <iostream>

namespace color
{
    constexpr std::string RED = "\033[31m";
    constexpr std::string GREEN = "\033[32m";
    constexpr std::string YELLOW = "\033[33m";
    constexpr std::string BLUE = "\033[34m";
    constexpr std::string RESET = "\033[0m";   
}

void ThreadLoggerGeneric::Log(const std::string& str)
{
    std::cout << str << '\n';
}

void ThreadLoggerGeneric::Warning(const std::string& str)
{
    std::cout << color::YELLOW << str << color::RESET << '\n';
}
