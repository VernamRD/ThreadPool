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

        template <class TFunction, class... TArgs>
        TaskBase(TFunction&& func, TArgs&&... args, std::source_location _context = std::source_location::current())
        {
            auto args_tuple = std::make_tuple(std::forward<TArgs>(args)...);
            auto func_copy = std::forward<TFunction>(func);

            executor = [func_copy, args_tuple]() -> void
            {
                std::apply(func_copy, args_tuple); 
            };
        }

        std::future<void> get_future()
        {
            return promise.get_future();
        }
        
        void execute()
        {
            executor();
            promise.set_value();
        }

    private:
        std::promise<void> promise;
        std::function<void()> executor;
    };
    
    class Task
    {
    public:
        Task();

        template <class TFunction, class... TArgs>
        void init_task(TFunction&& func, TArgs&&... args, threadpool::ETaskPriority task_priority, std::source_location _context = std::source_location::current())
        {
            //std::make_unique<TaskBase>(std::forward<TFunction>(func), std::forward<TArgs>(args));
        }

        template <class TFunction, class... TArgs>
        Task(TFunction&& func, TArgs&&... args, std::source_location _context = std::source_location::current())
        {
            auto args_tuple = std::make_tuple(std::forward<TFunction>(func), std::forward<TArgs>(args)...);
            auto func_copy = std::forward<TFunction>(func);

            task = [func_copy, args_tuple]() -> std::any
            {
                return std::apply(func_copy, args_tuple); 
            };
        }

        std::unique_ptr<TaskBase> task;
    };
}
