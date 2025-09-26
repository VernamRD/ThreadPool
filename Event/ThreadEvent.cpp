#include "ThreadEvent.h"

threadpool::ThreadEvent::ThreadEvent()
    : b_signaled(false)
{
}

void threadpool::ThreadEvent::trigger()
{
    std::unique_lock lock(mutex);
    b_signaled = true;

    condition.notify_all();
}

void threadpool::ThreadEvent::wait()
{
    std::unique_lock lock(mutex);

    if (b_signaled) return;

    condition.wait(lock);
}

void threadpool::ThreadEvent::reset()
{
    b_signaled = false;
}

bool threadpool::ThreadEvent::is_signaled() const
{
    std::unique_lock lock(mutex);
    return b_signaled;
}
