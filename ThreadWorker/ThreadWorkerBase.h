#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include <expected>
#include <mutex>

#include "IThreadWorker.h"
#include "../Event/ThreadEvent.h"

namespace threadpool
{
    struct ThreadWorkerInitData
    {
        
    };
    
    class ThreadWorkerBase : public IThreadWorker
    {
    public:
        ThreadWorkerBase() = delete;
        ThreadWorkerBase(ThreadWorkerInitData& init_data);
        ThreadWorkerBase(const ThreadWorkerBase& other) = delete;
        ThreadWorkerBase(ThreadWorkerBase&& other) noexcept = delete;
        ThreadWorkerBase& operator=(const ThreadWorkerBase& other) = delete;
        ThreadWorkerBase& operator=(ThreadWorkerBase&& other) noexcept = delete;
        ~ThreadWorkerBase() override;

        static std::expected<std::shared_ptr<ThreadWorkerBase>, std::string> create(ThreadWorkerInitData& init_data);
        
        std::expected<bool, std::string> start() override;
        void stop() override;
        void stop_immediately();

        virtual bool is_want_to_stop() const override;

    private:
        virtual void initialize_on_main_thread();
        
        void run();
        
        std::expected<bool, std::string> initialize() override;
        int32_t main() override;
        void deinitialize() override;

        ThreadEvent thread_event;
        std::atomic<bool> b_running;
        std::atomic<bool> b_want_stop;
        
        std::thread thread;
    };
}
