
#include <iostream>
#include <ostream>
#include <thread>

#include "ThreadPool/ThreadPool.h"

int main(int argc, char* argv[])
{
    auto pool = threadpool::ThreadPool::initialize(threadpool::ThreadPoolInitData());

    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool->stop_all_immediately();
    
    return 0;
}
