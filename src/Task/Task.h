#pragma once
#include <any>
#include <functional>
#include <future>
#include <source_location>
#include <tuple>

#include "../Pool/ThreadPoolTypes.h"

namespace threadpool
{
    enum class ETaskPriority : uint8_t
    {
        Highest = 0,
        Normal = 1,
        Lowest = 2,
    };

    class TaskBase
    {
    public:
        TaskBase() = delete;
        TaskBase(const TaskBase& other) = delete;
        TaskBase& operator=(const TaskBase& other) = delete;

        TaskBase(TaskBase&& other) noexcept = default;
        TaskBase& operator=(TaskBase&& other) noexcept = default;

    protected:
        template <class TFunction, class... TArgs>
        explicit TaskBase(TFunction&& func, TArgs&&... args)
        {
            auto args_tuple = std::make_tuple(std::forward<TArgs>(args)...);
            auto func_copy = std::forward<TFunction>(func);

            executor = [func_copy, args_tuple]() -> void
            {
                std::apply(func_copy, args_tuple);
            };
        }

    public:
        void execute()
        {
            executor();
            promise.set_value();
        }
        
        std::future<void> get_future()
        {
            return promise.get_future();
        }

    private:
        std::promise<void> promise;
        std::function<void()> executor;
    };

    class Task;

    class TaskHandle
    {
    public:
        TaskHandle() = delete;

        explicit TaskHandle(Task* handled_task)
            : task(handled_task) {}

        [[nodiscard]] std::future<void> get_future() const;
        [[nodiscard]] const Task* get_task() const;
        [[nodiscard]] bool is_stale() const;

    private:
        void reset();
        Task* task;

        friend Task;
    };

    typedef std::shared_ptr<threadpool::TaskHandle> TaskHandlePtr;

    class Task : public TaskBase
    {
    public:
        Task() = delete;
        Task(const Task& other) = delete;
        Task& operator=(const Task& other) = delete;

        Task(Task&& other) noexcept = default;
        Task& operator=(Task&& other) noexcept = default;

        template <class TFunction, class... TArgs>
        explicit Task(TFunction&& func, TArgs&&... args, const std::source_location current_context = std::source_location::current()) noexcept
            : TaskBase(func, std::forward<TArgs>(args)...),
              context(current_context)
        {
            handle = std::make_unique<TaskHandle>(this);
        }

        ~Task()
        {
            handle->reset();
        }

        [[nodiscard]] TaskHandlePtr get_handle() const { return handle; }

    private:
        std::source_location context;
        TaskHandlePtr handle;
    };
}