#pragma once

#include "IThreadLogger.h"

class ThreadLoggerGeneric : public IThreadLogger
{
public:
    void Log(const std::string& str) override;
    void Warning(const std::string& str) override;
};
