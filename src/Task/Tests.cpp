
#include "Task.h"
#include "TaskPipe.h"
#include "gtest/gtest.h"

TEST(Task, CreateAndExecute)
{
    bool b_executed = false;
    
    auto Task = std::make_unique<threadpool::Task>([&b_executed]()
    {
        b_executed = true;
    });

    EXPECT_FALSE(b_executed);
    Task->execute();
    EXPECT_TRUE(b_executed);
}

TEST(Pipe, Execute)
{
    threadpool::TaskPipe pipe;
    
    constexpr int32_t task_count = 100;
    std::array<bool, task_count> task_complete{};

    for (int32_t i = 0; i < task_count; ++i)
    {
        pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([i, &task_complete]()
       {
           task_complete[i] = true;
       }));   
    }

    for (int32_t i = 0; i < task_count; ++i)
    {
        EXPECT_FALSE(task_complete[i]);   
    }

    pipe.do_job_until_task_exists();
    
    for (int32_t i = 0; i < task_count; ++i)
    {
        EXPECT_TRUE(task_complete[i]);   
    }
}

TEST(Pipe, ExecuteForTime)
{
    threadpool::TaskPipe pipe;

    constexpr int32_t task_count = 50;
    std::array<bool, task_count> task_complete{};

    constexpr std::chrono::milliseconds task_sleep_time(2);
    constexpr std::chrono::milliseconds job_time(10);
    constexpr int32_t expected_complete_task_count = job_time / task_sleep_time;

    for (int32_t i = 0; i < task_count; ++i)
    {
        pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([i, &task_complete, task_sleep_time]()
       {
           task_complete[i] = true;
           std::this_thread::sleep_for(task_sleep_time);
       }));   
    }

    for (int32_t i = 0; i < task_count; ++i)
    {
        EXPECT_FALSE(task_complete[i]);
    }
    
    pipe.do_job(job_time);

    int32_t complete_count = 0;
    for (int32_t i = 0; i < task_count; ++i)
    {
        complete_count += task_complete[i] ? 1 : 0;
    }
    
    EXPECT_TRUE(complete_count <= expected_complete_task_count);
}

TEST(Pipe, ExecutePriority)
{
    threadpool::TaskPipe pipe;

    struct ExecutedTasks
    {
        bool b_executed_highest_task = false;
        bool b_executed_normal_task = false;
        bool b_executed_lowest_task = false;  
    };

    ExecutedTasks executed_tasks;
    
    pipe.add_task(threadpool::ETaskPriority::Highest, std::make_unique<threadpool::Task>([&executed_tasks]()
    {
        EXPECT_FALSE(executed_tasks.b_executed_normal_task);
        EXPECT_FALSE(executed_tasks.b_executed_lowest_task);
        executed_tasks.b_executed_highest_task = true;
    }));
    pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([&executed_tasks]()
    {
        EXPECT_TRUE(executed_tasks.b_executed_highest_task);
        EXPECT_FALSE(executed_tasks.b_executed_lowest_task);
        executed_tasks.b_executed_normal_task = true;
    }));
    pipe.add_task(threadpool::ETaskPriority::Lowest, std::make_unique<threadpool::Task>([&executed_tasks]()
    {
        EXPECT_TRUE(executed_tasks.b_executed_highest_task);
        EXPECT_TRUE(executed_tasks.b_executed_normal_task);
        executed_tasks.b_executed_lowest_task = true;
    }));

    pipe.do_job_until_task_exists();
    EXPECT_TRUE(executed_tasks.b_executed_highest_task);
    EXPECT_TRUE(executed_tasks.b_executed_normal_task);
    EXPECT_TRUE(executed_tasks.b_executed_lowest_task);
}

TEST(Pipe, Context)
{
    threadpool::TaskPipe pipe;

    threadpool::TaskHandlePtr handle_0;
    threadpool::TaskHandlePtr handle_1;
    threadpool::TaskHandlePtr handle_2;
    
    handle_0 = pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([&handle_0]()
    {
        EXPECT_NE(handle_0, nullptr);
        EXPECT_TRUE(threadpool::TaskContext::get().task().get() == handle_0->get_task());
    }));
    
    handle_1 = pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([&handle_1]()
    {
        EXPECT_NE(handle_1, nullptr);
        EXPECT_TRUE(threadpool::TaskContext::get().task().get() == handle_1->get_task());
    }));

    handle_2 = pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([&handle_2]()
    {
        EXPECT_NE(handle_2, nullptr);
        EXPECT_TRUE(threadpool::TaskContext::get().task().get() == handle_2->get_task());
    }));

    pipe.do_job_until_task_exists();
}