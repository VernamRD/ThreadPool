#pragma once

#include <cstdint>
#include <string>

namespace threadpool
{
    struct ThreadPoolInitData
    {
        std::string pool_name = "unnamed"; 
        int32_t numThreads = 16;
    };

    class IThreadPool
    {
    protected:
        ~IThreadPool() = default;

    public:
        virtual void stop_all() = 0;
        virtual void stop_all_immediately() = 0;

        virtual bool is_in_pool() const = 0;
    };
}
