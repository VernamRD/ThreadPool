#include "Event.h"

threadpool::Event::Event()
    : b_signaled(false)
{
}

void threadpool::Event::trigger()
{
    std::unique_lock lock(mutex);
    b_signaled = true;

    condition.notify_all();
    stall_counter = 0;
}

void threadpool::Event::wait()
{
    std::unique_lock lock(mutex);

    if (b_signaled) return;

    ++stall_counter;
    condition.wait(lock);
}

void threadpool::Event::reset()
{
    b_signaled = false;
}

bool threadpool::Event::is_signaled() const
{
    std::unique_lock lock(mutex);
    return b_signaled;
}

bool threadpool::Event::is_waiting() const
{
    std::unique_lock lock(mutex);
    return stall_counter > 0;
}
