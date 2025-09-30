#include "ThreadWorker.h"
#include "gtest/gtest.h"

class ThreadWorkerTest : public threadpool::ThreadWorker
{
public:
    explicit ThreadWorkerTest(const threadpool::ThreadWorkerInitData& init_data)
        : ThreadWorker(init_data)
    {
    }

    int32_t main() override
    {
        while (!is_want_to_stop())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        
        return 0;
    }
};

TEST(Worker, CreateAndStop)
{
    auto worker_init_data = threadpool::ThreadWorkerInitData();
    auto new_worker = ThreadWorkerTest::create(worker_init_data);
    EXPECT_NE(new_worker, nullptr);

    EXPECT_FALSE(new_worker->is_running());
    EXPECT_FALSE(new_worker->is_want_to_stop());

    new_worker->start();
    EXPECT_TRUE(new_worker->is_running());
    EXPECT_TRUE(new_worker->is_exist_thread());
    EXPECT_FALSE(new_worker->is_want_to_stop());

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    new_worker->stop();
    EXPECT_TRUE(new_worker->is_want_to_stop());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_FALSE(new_worker->is_running());
}

TEST(Worker, CreateAndStopImmediately)
{
    auto worker_init_data = threadpool::ThreadWorkerInitData();
    auto new_worker = threadpool::ThreadWorker::create(worker_init_data);
    EXPECT_NE(new_worker, nullptr);

    EXPECT_FALSE(new_worker->is_running());
    EXPECT_FALSE(new_worker->is_want_to_stop());

    new_worker->start();
    EXPECT_TRUE(new_worker->is_running());
    EXPECT_FALSE(new_worker->is_want_to_stop());

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    new_worker->stop_immediately();
    EXPECT_FALSE(new_worker->is_running());
    EXPECT_FALSE(new_worker->is_exist_thread());
}
