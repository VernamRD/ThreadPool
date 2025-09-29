#include "ThreadPool.h"

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
            auto start_result = new_worker->start();

            LOG(Verbose, "ThreadPool:{} - Created worker {} with name({})", init_data.pool_name, i, new_worker->get_name());
        }
        else
        {
            //_ASSERT_EXPR(false, "Couldn't create ThreadPool worker");
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

bool threadpool::ThreadPool::is_in_pool(const std::thread::id thread_id) const
{
    for (const auto& worker : workers)
    {
        if (worker->get_id() == thread_id) return true;
    }

    return false;
}

void threadpool::ThreadPool::add_worker(const std::shared_ptr<ThreadWorker>& new_worker)
{
    workers.push_back(new_worker);
}
