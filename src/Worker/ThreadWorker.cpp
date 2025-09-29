#include "ThreadWorker.h"

#include <format>
#include <shared_mutex>

#include "ILogger.h"

threadpool::ThreadWorker::ThreadWorker(const ThreadWorkerInitData& init_data)
    : b_running(false),
      b_want_stop(false),
      b_exist_thread(false)
{
    data.worker_name = init_data.worker_name;
}

threadpool::ThreadWorker::~ThreadWorker()
{
    if (b_exist_thread)
    {
        stop_immediately();
    }
}

std::shared_ptr<threadpool::ThreadWorker> threadpool::ThreadWorker::create(ThreadWorkerInitData& init_data)
{
    return std::make_shared<ThreadWorker>(init_data);
}

bool threadpool::ThreadWorker::start()
{
    if (b_running) return true;

    create_thread();

    initialize_on_main_thread();

    b_running = true;
    thread_event.trigger();

    return true;
}

void threadpool::ThreadWorker::stop()
{
    b_want_stop = true;
}

void threadpool::ThreadWorker::stop_immediately()
{
    // _ASSERT_EXPR(std::this_thread::get_id() != thread.get_id(), "Attempted stop_immediately from same thread"));

    if (b_running)
    {
        LOG(Verbose, "ThreadWorker: Requested immediately stop for WorkerThread({})", data.worker_name);
    }

    destroy_thread();
}

std::thread::id threadpool::ThreadWorker::get_id() const
{
    return data.thread_id;
}

std::string threadpool::ThreadWorker::get_name() const
{
    return data.worker_name;
}

bool threadpool::ThreadWorker::is_running() const
{
    return b_running;
}

bool threadpool::ThreadWorker::is_want_to_stop() const
{
    return b_want_stop;
}

bool threadpool::ThreadWorker::is_exist_thread() const
{
    return b_exist_thread;
}

void threadpool::ThreadWorker::initialize_on_main_thread()
{
    // _ASSERT_EXPR(std::this_thread::get_id() != thread.get_id(),
                 // "Attempted call initialize_on_main_thread when thread is null");
}

void threadpool::ThreadWorker::run()
{
    thread_event.wait();

    bool b_initialized = initialize().value_or(false);
    if (!b_initialized)
    {
        LOG(Verbose, "ThreadWorker: WorkerThread({}) couldn't initialized", data.worker_name);
        return;
    }

    int32_t result = -1;
    if (!is_want_to_stop())
    {
        LOG(Verbose, "ThreadWorker: WorkerThread({}) in run", data.worker_name);

        result = main();
    }

    LOG(Verbose, "ThreadWorker: WorkerThread({}) complete with result: {}", data.worker_name, result);

    deinitialize();
}

std::expected<bool, std::string> threadpool::ThreadWorker::initialize()
{
    return true;
}

int32_t threadpool::ThreadWorker::main()
{
    while (!is_want_to_stop())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}

void threadpool::ThreadWorker::deinitialize()
{
    LOG(Verbose, "ThreadWorker: WorkerThread({}) deinitializing", data.worker_name);
    b_running = false;
    b_want_stop = false;
    
}

void threadpool::ThreadWorker::create_thread()
{
    // _ASSERT_EXPR(!b_exist_thread, "Attempted create thread when already exist");

    std::unique_lock lock(thread_mutex);
    thread = std::thread(&ThreadWorker::run, this);
    data.thread_id = thread.get_id();
    b_exist_thread = true;
}

void threadpool::ThreadWorker::destroy_thread()
{
    // _ASSERT_EXPR(b_exist_thread, "Attempted destroy not exist thread");

    LOG(Verbose, "ThreadWorker: WorkerThread({}) destroying thread...", data.worker_name);

    if (!is_want_to_stop())
    {
        b_want_stop = true;
    }

    std::unique_lock lock(thread_mutex);
    if (thread.joinable())
    {
        thread.join();
        LOG(Verbose, "ThreadWorker: WorkerThread({}) thread destroyed success", data.worker_name);
        b_exist_thread = false;
    }
}
