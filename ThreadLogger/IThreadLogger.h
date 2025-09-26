#pragma once

#include <string>

class IThreadLogger
{
public:
    virtual ~IThreadLogger() = default;
    static IThreadLogger& get();

    virtual void Verbose(const std::string& str) = 0;
    virtual void Log(const std::string& str) = 0;
    virtual void Warning(const std::string& str) = 0;
};
