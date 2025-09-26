#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace threadpool
{
    enum class ETaskPriority : uint8_t
    {
        Highest = 0,
        Normal = 1,
        Lowest = 2,
    };
    
    struct ThreadPoolInitData
    {
        std::string name; 
        int32_t numThreads = 5;
    };

    class IThreadPool
    {
    protected:
        ~IThreadPool() = default;

    public:
        virtual void stop_all() = 0;
        virtual void stop_all_immediately() = 0;
    };
}
