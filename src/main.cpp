
#include <memory>
#include <thread>

#define ENABLE_LOG 1

#if ENABLE_LOG
#include "Logger/ILogger.h"
#endif
// #include "ThreadPool/ThreadPool.h"
// #include "ThreadWorkerTask/ThreadWorkerTaskBase.h"

int main(int argc, char* argv[])
{
    // auto pool = threadpool::ThreadPool::initialize(threadpool::ThreadPoolInitData());
    //
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    //
    // auto task = threadpool::TaskBase([pool]()
    // {
    //     pool->stop_all_immediately();
    // });
    //
    // task.execute();

#if ENABLE_LOG
    auto Logger = std::make_shared<threadpool::ConsoleLogger>();
    threadpool::LoggerManager::set_logger(Logger);
    LOG(Warning, "Hello World");
#endif
    
    return 0;
}
