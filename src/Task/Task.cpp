#include "Task.h"

std::future<void> threadpool::TaskHandle::get_future() const
{
    return task->get_future();
}

const threadpool::Task* threadpool::TaskHandle::get_task() const
{
    return task;
}

bool threadpool::TaskHandle::is_stale() const
{
    return task != nullptr;
}

void threadpool::TaskHandle::reset()
{
    task = nullptr;
}