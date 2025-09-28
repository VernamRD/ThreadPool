

#include "ILogger.h"
#include "gtest/gtest.h"

void init_logger()
{
    typedef threadpool::ConsoleLogger LoggerClass;
    
    auto Logger = std::make_shared<LoggerClass>();
    threadpool::LoggerManager::set_logger(Logger);
}

TEST(LoggerTest, Verbose)
{
    init_logger();
    LOG(Verbose, "LoggerTest:Verbose");
    LOG(Verbose, "LoggerTest:Verbose format{}{}{}", "Number", 147, 147.f);
}

TEST(LoggerTest, Info)
{
    init_logger();
    LOG(Info, "LoggerTest:Info");
    LOG(Info, "LoggerTest:Info format{}{}{}", "Number", 147, 147.f);
}

TEST(LoggerTest, Log)
{
    init_logger();
    LOG(Log, "LoggerTest:Log");
    LOG(Log, "LoggerTest:Log format{}{}{}", "Number", 147, 147.f);
}

TEST(LoggerTest, Warning)
{
    init_logger();
    LOG(Warning, "LoggerTest:Warning");
    LOG(Warning, "LoggerTest:Warning format{}{}{}", "Number", 147, 147.f);
}

TEST(LoggerTest, Error)
{
    init_logger();
    LOG(Error, "LoggerTest:Error");
    LOG(Error, "LoggerTest:Error format{}{}{}", "Number", 147, 147.f);
}