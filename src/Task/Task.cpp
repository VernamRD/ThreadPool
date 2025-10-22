#include "Task.h"

std::future<void> threadpool::TaskHandle::get_future() const
{
    return task->get_future();
}

const threadpool::Task* threadpool::TaskHandle::get_task() const
{
    return task;
}

bool threadpool::TaskHandle::is_valid() const
{
    return task != nullptr;
}

bool threadpool::TaskHandle::is_stale() const
{
    return task == nullptr;
}

bool threadpool::TaskHandle::is_completed() const
{
    return is_stale() || task->is_completed();
}

void threadpool::TaskHandle::reset()
{
    task = nullptr;
}