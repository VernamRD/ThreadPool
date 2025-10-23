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

TEST(Pool, Multithread_Approve)
{
    constexpr int32_t num_workers = 16;
    constexpr int32_t num_tasks_per_worker = 50;
    auto pool = create_and_start_thread_pool(num_workers);

    auto pipe = std::make_shared<threadpool::TaskPipe>();
    pool->set_pipe(pipe);
    pool->pause_all();

    std::mutex mutex;
    std::atomic<bool> b_multithread_access(false);

    constexpr std::chrono::milliseconds sleep_time = std::chrono::milliseconds(1);
    constexpr 
    std::chrono::milliseconds expected_work_time = sleep_time * num_tasks_per_worker * 2;
    
    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        for (int32_t task_i = 0; task_i < num_tasks_per_worker; ++task_i)
        {
            pipe->add_task(threadpool::ETaskPriority::Normal, [&sleep_time, &pool, task_i, &mutex, &b_multithread_access]()
            {
                std::unique_lock lock(mutex, std::defer_lock);
                
                bool b_not_locked = !lock.try_lock();
                if (b_not_locked)
                {
                    b_multithread_access.store(b_multithread_access.load() || b_not_locked);
                }
                
                std::this_thread::sleep_for(sleep_time);
            });
        }
    }

    auto current_time = std::chrono::system_clock::now();
    pool->unpause_all();
    pipe->wait_until_task_exists(std::chrono::seconds(1));
    EXPECT_TRUE(b_multithread_access);
    EXPECT_LT(std::chrono::system_clock::now() - current_time, expected_work_time);
}

TEST(Pool, Create_4_Tasks_10_With_Pause)
{
    constexpr int32_t num_workers = 4;
    constexpr int32_t num_tasks_per_worker = 10;
    constexpr int32_t total_task_count = num_workers * num_tasks_per_worker;

    auto pool = create_and_start_thread_pool(num_workers);

    auto pipe = std::make_shared<threadpool::TaskPipe>();
    pool->set_pipe(pipe);
    
    pool->pause_all();

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

    EXPECT_TRUE(pool->is_paused());
    EXPECT_EQ(pipe->get_tasks_all_count(), total_task_count);
    EXPECT_EQ(pipe->get_tasks_pending_count(), total_task_count);
    EXPECT_EQ(pipe->get_tasks_in_progress_count(), 0);
    pool->unpause_all();
    pipe->wait_until_task_exists(std::chrono::seconds(1));

    int32_t completed_tasks_count = 0;
    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        completed_tasks_count += completed_tasks[worker_i];
    }

    EXPECT_EQ(pipe->get_tasks_all_count(), 0);
    EXPECT_EQ(pipe->get_tasks_pending_count(), 0);
    EXPECT_EQ(pipe->get_tasks_in_progress_count(), 0);
    EXPECT_EQ(completed_tasks_count, total_task_count);
}

TEST(Pool, Create_16_Tasks_100)
{
    constexpr int32_t num_workers = 16;
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

    pipe->wait_until_task_exists(std::chrono::seconds(1));

    int32_t completed_tasks_count = 0;
    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        completed_tasks_count += completed_tasks[worker_i];
    }

    EXPECT_EQ(completed_tasks_count, total_task_count);
}

TEST(Pool, Create_32_Tasks_1000)
{
    constexpr int32_t num_workers = 32;
    constexpr int32_t num_tasks_per_worker = 1000;
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

    pipe->wait_until_task_exists(std::chrono::seconds(10));

    int32_t completed_tasks_count = 0;
    for (int32_t worker_i = 0; worker_i < num_workers; ++worker_i)
    {
        completed_tasks_count += completed_tasks[worker_i];
    }

    EXPECT_EQ(completed_tasks_count, total_task_count);
}