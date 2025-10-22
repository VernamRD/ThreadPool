#pragma once

#include <cstdint>
#include <string>
#include <shared_mutex>

namespace threadpool
{
    class TaskPipe;

    struct ThreadPoolInitData
    {
        std::string pool_name = "unnamed"; 
        int32_t numThreads = 16;
    };

    class IThreadPool
    {
    protected:
        ~IThreadPool() = default;

    public:
        /** Run all workers in pool */
        virtual void start() = 0;

        /** Request stop all workers in pool */
        virtual void stop_all() = 0;
        /** Request stop all workers in pool. @remark Calling thread will wait for each of the workers to stop. */
        virtual void stop_all_immediately() = 0;

        virtual void set_pipe(std::shared_ptr<TaskPipe> new_task_pipe) = 0;
        [[nodiscard]] virtual std::shared_lock<std::shared_mutex> get_pipe(std::shared_ptr<TaskPipe>& pipe_ptr) const = 0;
        
        [[nodiscard]] virtual bool is_in_pool(std::thread::id id) const = 0;
        [[nodiscard]] virtual int32_t get_worker_id(std::thread::id id) const = 0;
    };
}
