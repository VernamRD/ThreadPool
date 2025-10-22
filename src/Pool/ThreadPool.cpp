#include "ThreadPool.h"

#include <algorithm>
#include <format>

#include "ILogger.h"
#include "ThreadWorker.h"

threadpool::ThreadPool::ThreadPool()
{}

threadpool::ThreadPool::~ThreadPool()
{
    ThreadPool::stop_all_immediately();
}

std::shared_ptr<threadpool::IThreadPool> threadpool::ThreadPool::create(
    const threadpool::ThreadPoolInitData& init_data)
{
    LOG(Log, "Initialize ThreadPool - {}", init_data.pool_name);

    auto new_pool = std::make_shared<ThreadPool>();

    for (int32_t i = 0; i < init_data.numThreads; ++i)
    {
        auto worker_init_data = ThreadWorkerInitData();
        worker_init_data.owner_pool = new_pool;
        if (auto new_worker = ThreadWorker::create(worker_init_data))
        {
            new_pool->add_worker(new_worker);

            LOG(Verbose, "ThreadPool:{} - Created worker {} with name({})", init_data.pool_name, i, new_worker->get_name());
        }
        else
        {
            LOG(Warning, "ThreadPool:{} - couldn't create worker by index {}", init_data.pool_name, i);
        }
    }

    return new_pool;
}

void threadpool::ThreadPool::start()
{
    for (auto& worker : workers)
    {
        worker->start();
    }
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

void threadpool::ThreadPool::set_pipe(std::shared_ptr<TaskPipe> new_task_pipe)
{
    std::unique_lock pipe_lock(pipe_mutex);

    pipe = new_task_pipe;
}

std::shared_lock<std::shared_mutex> threadpool::ThreadPool::get_pipe(std::shared_ptr<TaskPipe>& pipe_ptr) const
{
    std::shared_lock pipe_lock(pipe_mutex);
    pipe_ptr = pipe;
    return std::move(pipe_lock);
}

bool threadpool::ThreadPool::is_in_pool(std::thread::id thread_id) const
{
    return std::ranges::any_of(workers, [&thread_id](const auto& worker)
    {
        return worker->get_id() == thread_id;
    });
}

int32_t threadpool::ThreadPool::get_worker_id(std::thread::id id) const
{
    for (int32_t i = 0; i < workers.size(); ++i)
    {
        if (workers[i]->get_id() == id)
        {
            return i;
        }
    }
    
    return -1;
}

void threadpool::ThreadPool::add_worker(const std::shared_ptr<ThreadWorker>& new_worker)
{
    workers.push_back(new_worker);
}
