#pragma once

#include <map>
#include <shared_mutex>
#include <memory>
#include "Task.h"

namespace threadpool
{
    using TaskPtr = std::shared_ptr<Task>;
    using ConstTaskPtr = std::shared_ptr<const Task>;
    
    class TaskContext
    {
    public:
        static const TaskContext& get();

        TaskContext()
            : _task(nullptr),
              _priority(ETaskPriority::Highest) {}

        TaskContext(const TaskPtr& task, ETaskPriority priority)
            : _task(task),
              _priority(priority) {}

        ConstTaskPtr task() const { return _task; }
        ETaskPriority priority() const { return _priority; }

    private:
        ConstTaskPtr _task;
        ETaskPriority _priority;

        friend class TaskPipe;
    };

    class TaskPipe
    {
    public:
        std::shared_ptr<TaskHandle> add_task(ETaskPriority task_priority, std::unique_ptr<threadpool::Task>&& task);

        void do_job_until_task_exists();

        template <class _Rep, class _Period>
        void do_job(std::chrono::duration<_Rep, _Period> duration = std::chrono::milliseconds(10)) {}

    private:
        void set_context(const TaskContext& context);
        TaskPtr pop_relevant_task(TaskContext& context);

        std::shared_mutex mutex;
        std::map<ETaskPriority, std::vector<TaskPtr>> tasks;
    };
}