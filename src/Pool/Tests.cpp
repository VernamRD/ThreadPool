#include "TaskPipe.h"
#include "ThreadPool.h"
#include "gtest/gtest.h"

std::shared_ptr<threadpool::IThreadPool> create_and_start_thread_pool(int32_t num_workers)
{
    threadpool::ThreadPoolInitData init_data;
    init_data.pool_name = "Test pool";
    init_data.numThreads = num_workers;

    auto pool = threadpool::ThreadPool::create(init_data);
    pool->start();

    return std::move(pool);
}

TEST(Pool, Initialize_4_And_Stop)
{
    constexpr int32_t num_workers = 4;
    auto pool = create_and_start_thread_pool(num_workers);
    pool->stop_all_immediately();
}

TEST(Pool, Create_16_Tasks_100)
{
    constexpr int32_t num_workers = 1;
    constexpr int32_t num_tasks_per_worker = 100;
    constexpr int32_t total_task_count = num_workers * num_tasks_per_worker;

    auto pool = create_and_start_thread_pool(num_workers);

    auto pipe = std::make_shared<threadpool::TaskPipe>();
    pool->set_pipe(pipe);

    std::array<int32_t, num_workers> completed_tasks{};

    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        for (int32_t task_i = 0; task_i < num_tasks_per_worker; ++task_i)
        {
            EXPECT_EQ(completed_tasks[worker_i], 0);
        }
    }

    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        for (int32_t task_i = 0; task_i < num_tasks_per_worker; ++task_i)
        {
            pipe->add_task(threadpool::ETaskPriority::Normal, [&pool, task_i, &completed_tasks]()
            {
                ++completed_tasks[pool->get_worker_id(std::this_thread::get_id())];
            });
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    int32_t completed_tasks_count = 0;
    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        completed_tasks_count += completed_tasks[worker_i];
    }

    EXPECT_EQ(completed_tasks_count, total_task_count);
}