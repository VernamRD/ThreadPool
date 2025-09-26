#pragma once

#include <expected>
#include <string>

namespace threadpool
{
    class IThreadWorker
    {
    public:
        virtual ~IThreadWorker() = default;
        
        virtual std::expected<bool, std::string> start() = 0;
        virtual void stop() = 0;

        virtual std::thread::id get_id() const = 0;
        virtual bool is_want_to_stop() const = 0;

    protected:
        virtual std::expected<bool, std::string> initialize() = 0;
        virtual int32_t main() = 0;
        virtual void deinitialize() = 0;
    };   
}
