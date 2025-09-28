#pragma once

#include <expected>
#include <string>
#include <thread>

namespace threadpool
{
    class IThreadWorker
    {
    public:
        virtual ~IThreadWorker() = default;
        
        virtual bool start() = 0;
        virtual void stop() = 0;

        virtual std::thread::id get_id() const = 0;
        virtual std::string get_name() const = 0;

        virtual bool is_running() const = 0;
        virtual bool is_want_to_stop() const = 0;
        virtual bool is_exist_thread() const = 0;

    protected:
        virtual std::expected<bool, std::string> initialize() = 0;
        virtual int32_t main() = 0;
        virtual void deinitialize() = 0;
    };   
}
