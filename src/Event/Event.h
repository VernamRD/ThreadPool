#pragma once

#include <condition_variable>

namespace threadpool
{
    class Event
    {
    public:
        Event();

        Event(const Event& other) = delete;
        Event& operator=(const Event& other) = delete;
        Event(Event&&) noexcept = delete;
        Event& operator=(Event&&) noexcept = delete;

        void trigger();
        void wait();
        void reset();

        bool is_signaled() const;
        bool is_waiting() const;
        
    private:
        mutable std::mutex mutex;
        std::condition_variable condition;
        std::atomic<bool> b_signaled;
        std::atomic<int32_t> stall_counter;
    };   
}
