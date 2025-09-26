#pragma once

#include <memory>
#include <vector>

#include "ThreadPoolTypes.h"

namespace threadpool
{
    class ThreadWorkerBase;

    class ThreadPool : public IThreadPool
    {
    public:
        virtual ~ThreadPool() { stop_all(); }
        
        static std::shared_ptr<IThreadPool> initialize(const threadpool::ThreadPoolInitData& init_data);
        virtual void stop_all() override;
        virtual void stop_all_immediately() override;

    private:
        void add_worker(std::shared_ptr<ThreadWorkerBase> new_worker);
        
        std::vector<std::shared_ptr<ThreadWorkerBase>> workers;
    };
}
