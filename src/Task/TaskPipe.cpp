#include "../Task/TaskPipe.h"

#include <algorithm>

thread_local threadpool::TaskContext task_context;

const threadpool::TaskContext& threadpool::TaskContext::get()
{
    return task_context;
}

threadpool::TaskPipe::TaskPipe()
{}

std::shared_ptr<threadpool::TaskHandle> threadpool::TaskPipe::add_task(threadpool::ETaskPriority task_priority,
    std::unique_ptr<threadpool::Task>&& task) noexcept
{
    std::unique_lock tasks_lock(tasks_mutex);
    auto cached_handle = task->get_handle();
    tasks[task_priority].push_back(std::forward<TaskPtr>(std::move(task)));
    return cached_handle;
}

void threadpool::TaskPipe::set_context(const TaskContext& context) noexcept
{
    task_context = context;
}

threadpool::TaskPtr threadpool::TaskPipe::pop_relevant_task(TaskContext& context) noexcept
{
    std::unique_lock lock(tasks_mutex);

    ETaskPriority current_priority;
    bool relevant_task_exists = false;

    for (uint8_t priority = std::numeric_limits<uint8_t>::min(); priority <= static_cast<uint8_t>(ETaskPriority::Lowest); ++priority)
    {
        current_priority = static_cast<ETaskPriority>(priority);
        if (tasks.contains(current_priority) && !tasks[current_priority].empty())
        {
            relevant_task_exists = true;
            break;
        }
    }

    if (!relevant_task_exists) return nullptr;

    auto task = std::move(tasks[current_priority].front());
    tasks[current_priority].erase(tasks[current_priority].begin());

    context._task = task;

    return std::move(task);
}