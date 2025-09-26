#include "ThreadPool.h"
#include "ThreadPoolTypes.h"
#include "../ThreadLogger/IThreadLogger.h"
#include "../ThreadWorker/ThreadWorkerBase.h"

std::shared_ptr<threadpool::IThreadPool> threadpool::ThreadPool::initialize(const threadpool::ThreadPoolInitData& init_data)
{
    IThreadLogger::get().Log("Initialize ThreadPool - " + init_data.name);
    
    auto new_pool = std::make_shared<ThreadPool>();
    
    for (int32_t i = 0; i < init_data.numThreads; ++i)
    {
        auto worker_init_data = ThreadWorkerInitData();
        auto create_result = ThreadWorkerBase::create(worker_init_data);
        if (create_result.has_value())
        {
            new_pool->add_worker(create_result.value());
            auto start_result = create_result.value()->start();
        }
    }

    return new_pool;
}

void threadpool::ThreadPool::stop_all()
{
    for (auto& worker : workers)
    {
        worker->stop();
    }
}

void threadpool::ThreadPool::stop_all_immediately()
{
    stop_all();

    for (auto& worker : workers)
    {
        worker->stop_immediately();
    }
}

void threadpool::ThreadPool::add_worker(std::shared_ptr<ThreadWorkerBase> new_worker)
{
    workers.push_back(new_worker);
}
