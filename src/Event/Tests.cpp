#include "Event.h"
#include "gtest/gtest.h"

TEST(EventTest, Sync)
{
    auto Event = std::make_shared<threadpool::Event>();
    auto thread = std::thread([Event]()
    {
        // Wait main thread
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        
        EXPECT_FALSE(Event->is_signaled());
        EXPECT_TRUE(Event->is_waiting());

        Event->trigger();
    });

    EXPECT_FALSE(Event->is_signaled());
    
    Event->wait();
    
    EXPECT_TRUE(Event->is_signaled());
    EXPECT_FALSE(Event->is_waiting());

    if (thread.joinable())
    {
        thread.join();
    }
}
