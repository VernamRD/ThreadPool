#pragma once

#include <map>
#include <memory>
#include <shared_mutex>
#include <thread>
#include <vector>

#include "ThreadPoolTypes.h"

namespace threadpool
{
    enum class ETaskPriority : uint8_t;
    class ThreadWorker;
    struct ThreadWorkerData;

    class ThreadPool : public IThreadPool
    {
    public:
        ThreadPool();
        virtual ~ThreadPool();

        static std::shared_ptr<IThreadPool> create(const threadpool::ThreadPoolInitData& init_data);

        void start() override;
        void stop_all() override;
        void stop_all_immediately() override;

        void set_pipe(std::shared_ptr<TaskPipe> new_task_pipe) override;
        [[nodiscard]] std::shared_lock<std::shared_mutex> get_pipe(std::shared_ptr<TaskPipe>& pipe_ptr) const override;
        
        [[nodiscard]] bool is_in_pool(std::thread::id thread_id) const override;
        [[nodiscard]] int32_t get_worker_id(std::thread::id id) const override;

    private:
        void add_worker(const std::shared_ptr<ThreadWorker>& new_worker);

        mutable std::shared_mutex pipe_mutex;
        std::shared_ptr<TaskPipe> pipe;
        std::vector<std::shared_ptr<ThreadWorker>> workers;
    };
}
