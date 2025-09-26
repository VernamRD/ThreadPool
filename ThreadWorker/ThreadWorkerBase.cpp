#include "ThreadWorkerBase.h"

#include "../ThreadLogger/IThreadLogger.h"

threadpool::ThreadWorkerBase::ThreadWorkerBase(ThreadWorkerInitData& init_data)
    : b_running(false),
      b_want_stop(false),
      b_exist_thread(false)
{
}

threadpool::ThreadWorkerBase::~ThreadWorkerBase()
{
    if (b_exist_thread)
    {
        stop_immediately();   
    }
}

std::shared_ptr<threadpool::ThreadWorkerBase> threadpool::ThreadWorkerBase::create(ThreadWorkerInitData& init_data)
{
    return std::make_shared<ThreadWorkerBase>(init_data);
}

std::expected<bool, std::string> threadpool::ThreadWorkerBase::start()
{
    if (b_running) return std::unexpected("Thread worker already running");

    create_thread();

    initialize_on_main_thread();

    b_running = true;
    thread_event.trigger();

    return true;
}

void threadpool::ThreadWorkerBase::stop()
{
    b_want_stop = true;
}

void threadpool::ThreadWorkerBase::stop_immediately()
{
    _ASSERT_EXPR(std::this_thread::get_id() != thread.get_id(), "Attempted stop_immediately from same thread");

    if (b_running)
    {
        IThreadLogger::get().Verbose(std::format("ThreadWorker: Requested immediately stop for WorkerThread({})", thread.get_id()));
    }

    destroy_thread();
}

std::thread::id threadpool::ThreadWorkerBase::get_id() const
{
    return thread.get_id();
}

bool threadpool::ThreadWorkerBase::is_want_to_stop() const
{
    return b_want_stop;
}

void threadpool::ThreadWorkerBase::initialize_on_main_thread()
{
    _ASSERT_EXPR(std::this_thread::get_id() != thread.get_id(),
                 "Attempted call initialize_on_main_thread when thread is null");
}

void threadpool::ThreadWorkerBase::run()
{
    thread_event.wait();

    bool b_initialized = initialize().value_or(false);
    if (!b_initialized)
    {
        IThreadLogger::get().Warning(std::format("ThreadWorker: WorkerThread({}) couldn't initialized", thread.get_id()));
        return;
    }

    if (!is_want_to_stop())
    {
        IThreadLogger::get().Verbose(std::format("ThreadWorker: WorkerThread({}) in run", thread.get_id()));

        int32_t result = main();
    }

    IThreadLogger::get().Verbose(std::format("ThreadWorker: WorkerThread({}) complete", thread.get_id()));

    deinitialize();
}

std::expected<bool, std::string> threadpool::ThreadWorkerBase::initialize()
{
    return true;
}

int32_t threadpool::ThreadWorkerBase::main()
{
    while (!is_want_to_stop())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}

void threadpool::ThreadWorkerBase::deinitialize()
{
    IThreadLogger::get().Verbose(std::format("ThreadWorker: WorkerThread({}) deinitializing", thread.get_id()));
    b_running = false;
}

void threadpool::ThreadWorkerBase::create_thread()
{
    _ASSERT_EXPR(!b_exist_thread, "Attempted create thread when already exist");

    thread = std::thread(&ThreadWorkerBase::run, this);
    b_exist_thread = true;
}

void threadpool::ThreadWorkerBase::destroy_thread()
{
    _ASSERT_EXPR(b_exist_thread, "Attempted destroy not exist thread");

    std::thread::id cached_thread_id = thread.get_id();
    IThreadLogger::get().Verbose(std::format("ThreadWorker: WorkerThread({}) destroying thread...", cached_thread_id));
    
    if (!is_want_to_stop())
    {
        b_want_stop = true;
    }
    
    if (thread.joinable())
    {
        thread.join();
        IThreadLogger::get().Log(std::format("ThreadWorker: WorkerThread({}) thread destroyed success", cached_thread_id));
        b_exist_thread = false;
    }
}

