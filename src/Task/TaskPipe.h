#pragma once

#include <map>
#include <shared_mutex>
#include <memory>
#include <set>

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

        [[nodiscard]] ConstTaskPtr task() const { return _task; }
        [[nodiscard]] ETaskPriority priority() const { return _priority; }

    private:
        ConstTaskPtr _task;
        ETaskPriority _priority;

        friend class TaskPipe;
    };

    class TaskPipe
    {
    public:
        TaskPipe();
        
        std::shared_ptr<TaskHandle> add_task(ETaskPriority task_priority, std::unique_ptr<threadpool::Task>&& task) noexcept;
        template <class TFunction, class... TArgs>
        std::shared_ptr<TaskHandle> add_task(ETaskPriority task_priority, TFunction&& func, TArgs&&... args,  const std::source_location current_context = std::source_location::current()) noexcept
        {
            return add_task(task_priority, std::make_unique<threadpool::Task>(func, args..., current_context));
        }

        void do_job_until_task_exists()
        {
            TaskContext context;
            while (auto task = pop_relevant_task(context))
            {
                set_context(context);
                task->execute();
            }
        }

        template <class Rep, class Period>
        void do_job(std::chrono::duration<Rep, Period> duration = std::chrono::milliseconds(10))
        {
            auto start_point = std::chrono::system_clock::now();
            
            TaskContext context;
            while (std::chrono::system_clock::now() - start_point < duration)
            {
                auto task = pop_relevant_task(context);
                if (task)
                {
                    set_context(context);
                    task->execute();   
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(50));   
                }
            }
        }

        [[nodiscard]] int32_t get_tasks_all_count() const noexcept;
        [[nodiscard]] int32_t get_tasks_in_progress_count() const noexcept;
        [[nodiscard]] int32_t get_tasks_pending_count() const noexcept;

    private:
        static void set_context(const TaskContext& context) noexcept;
        [[nodiscard]] TaskPtr pop_relevant_task(TaskContext& context) noexcept;
        
        mutable std::shared_mutex tasks_mutex;
        int32_t tasks_count;
        mutable std::vector<TaskHandlePtr> tasks_in_progress;
        std::map<ETaskPriority, std::vector<TaskPtr>> tasks;
    };
}