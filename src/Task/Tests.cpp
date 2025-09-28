
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

    bool b_executed_task = false;
    pipe.add_task(threadpool::ETaskPriority::Normal, std::make_unique<threadpool::Task>([&b_executed_task]()
    {
        b_executed_task = true;
    }));

    EXPECT_FALSE(b_executed_task);
    pipe.do_job_until_task_exists();
    EXPECT_TRUE(b_executed_task);
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