#include "ThreadWorkerBase.h"

threadpool::ThreadWorkerBase::ThreadWorkerBase(ThreadWorkerInitData& init_data)
    : b_running(false),
      b_want_stop(false)
{
}

threadpool::ThreadWorkerBase::~ThreadWorkerBase()
{
    stop_immediately();
}

std::expected<std::shared_ptr<threadpool::ThreadWorkerBase>, std::string> threadpool::ThreadWorkerBase::create(
    ThreadWorkerInitData& init_data)
{
    auto new_worker = std::make_shared<ThreadWorkerBase>(init_data);
    if (new_worker == nullptr) return std::unexpected("Couldn't allocate memory");

    return new_worker;
}

std::expected<bool, std::string> threadpool::ThreadWorkerBase::start()
{
    if (b_running) return std::unexpected("Thread worker already running");

    thread = std::thread(&ThreadWorkerBase::run, this);

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

    if (!b_want_stop)
    {
        stop();
    }

    if (thread.joinable())
    {
        thread.join();
    }
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
    if (!b_initialized) return;

    if (!is_want_to_stop())
    {
        int32_t result = main();
    }

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
}
