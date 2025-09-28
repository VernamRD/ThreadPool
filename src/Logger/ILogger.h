#pragma once

#include <format>
#include <string>
#include <memory>

namespace threadpool
{
    enum class LogLevel
    {
        Verbose = 0,
        Info = 1,
        Log = 2,
        Warning = 3,
        Error = 4,
    };

    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void log(LogLevel level, const std::string& message) = 0;
    };

    class ConsoleLogger : public ILogger
    {
        void log(LogLevel level, const std::string& message) override;
    };

    class NullLogger : public ILogger
    {
        void log(LogLevel level, const std::string& message) override
        {
            // Nothing
        }
    };

    class LoggerManager
    {
    public:
        static void set_logger(std::shared_ptr<ILogger> new_logger)
        {
            logger = new_logger;
        }

        static ILogger& get_logger()
        {
            if (!logger)
            {
                logger = std::make_shared<NullLogger>();
            }
            return *logger;
        }

    private:
        LoggerManager() = default;
        static std::shared_ptr<ILogger> logger;
    };

#define LOG(Category, String, ...) \
threadpool::LoggerManager::get_logger().log(threadpool::LogLevel::Category, std::format(String, __VA_ARGS__))
}