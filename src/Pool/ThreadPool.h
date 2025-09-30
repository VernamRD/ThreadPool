#pragma once

#include <map>
#include <memory>
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
        virtual ~ThreadPool() { stop_all(); }

        static std::shared_ptr<IThreadPool> initialize(const threadpool::ThreadPoolInitData& init_data);
        void stop_all() override;
        void stop_all_immediately() override;
        
        [[nodiscard]] bool is_in_pool() const override;

    private:
        void add_worker(const std::shared_ptr<ThreadWorker>& new_worker);

        std::vector<std::shared_ptr<ThreadWorker>> workers;
    };
}
