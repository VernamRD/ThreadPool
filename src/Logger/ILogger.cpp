#include "ILogger.h"

#include <iostream>

std::shared_ptr<threadpool::ILogger> threadpool::LoggerManager:: logger = nullptr;

namespace color
{
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string RESET = "\033[0m";
}

void threadpool::ConsoleLogger::log(LogLevel level, const std::string& message)
{
    switch (level)
    {
    case LogLevel::Verbose:
    case LogLevel::Info:
    case LogLevel::Log:
    default:
        std::cout << message << '\n';
        break;
    case LogLevel::Warning:
        std::cout << color::YELLOW << message << color::RESET << '\n';
        break;
    case LogLevel::Error:
        std::cout << color::RED << message << color::RESET << '\n';
        break;
    }
}
