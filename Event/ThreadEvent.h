#pragma once

#include <condition_variable>

namespace threadpool
{
    class ThreadEvent
    {
    public:
        ThreadEvent();

        ThreadEvent(const ThreadEvent& other) = delete;
        ThreadEvent& operator=(const ThreadEvent& other) = delete;
        ThreadEvent(ThreadEvent&&) noexcept = delete;
        ThreadEvent& operator=(ThreadEvent&&) noexcept = delete;

        void trigger();
        void wait();
        void reset();

        bool is_signaled() const;
        
    private:
        mutable std::mutex mutex;
        std::condition_variable condition;
        std::atomic<bool> b_signaled;
    };   
}
