#include "ThreadPool.h"

#include <algorithm>
#include <assert.h>
#include <format>

#include "ILogger.h"
#include "ThreadWorker.h"

std::shared_ptr<threadpool::IThreadPool> threadpool::ThreadPool::initialize(
    const threadpool::ThreadPoolInitData& init_data)
{
    LOG(Log, "Initialize ThreadPool - {}", init_data.pool_name);

    auto new_pool = std::make_shared<ThreadPool>();

    for (int32_t i = 0; i < init_data.numThreads; ++i)
    {
        auto worker_init_data = ThreadWorkerInitData();
        if (auto new_worker = ThreadWorker::create(worker_init_data))
        {
            new_pool->add_worker(new_worker);
            new_worker->start();

            LOG(Verbose, "ThreadPool:{} - Created worker {} with name({})", init_data.pool_name, i, new_worker->get_name());
        }
        else
        {
            LOG(Warning, "ThreadPool:{} - couldn't create worker by index {}", init_data.pool_name, i);
        }
    }

    return new_pool;
}

void threadpool::ThreadPool::stop_all()
{
    for (const auto& worker : workers)
    {
        worker->stop();
    }
}

void threadpool::ThreadPool::stop_all_immediately()
{
    stop_all();

    for (const auto& worker : workers)
    {
        worker->stop_immediately();
    }
}

bool threadpool::ThreadPool::is_in_pool() const
{
    const auto thread_id = std::this_thread::get_id();

    return std::ranges::any_of(workers, [&thread_id](const auto& worker)
    {
        return worker->get_id() == thread_id;
    });
}

void threadpool::ThreadPool::add_worker(const std::shared_ptr<ThreadWorker>& new_worker)
{
    workers.push_back(new_worker);
}
