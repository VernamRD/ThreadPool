#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <shared_mutex>

#include "IThreadWorker.h"
#include "Event.h"

namespace threadpool
{
    struct ThreadWorkerInitData
    {
        std::string worker_name;
    };

    struct ThreadWorkerData
    {
        std::string worker_name;
        std::thread::id thread_id;
    };
    
    class ThreadWorker : public IThreadWorker
    {
    public:
        explicit ThreadWorker(const ThreadWorkerInitData& init_data);
        
        ThreadWorker() = delete;
        ThreadWorker(const ThreadWorker& other) = delete;
        ThreadWorker(ThreadWorker&& other) noexcept = delete;
        ThreadWorker& operator=(const ThreadWorker& other) = delete;
        ThreadWorker& operator=(ThreadWorker&& other) noexcept = delete;
        
        ~ThreadWorker() override;

        static std::shared_ptr<threadpool::ThreadWorker> create(ThreadWorkerInitData& init_data);
        
        bool start() override;
        void stop() override;
        void stop_immediately();

        std::thread::id get_id() const override;
        std::string get_name() const override;

        bool is_running() const override;
        bool is_want_to_stop() const override;
        bool is_exist_thread() const override;

    private:
        virtual void initialize_on_main_thread();
        
        void run() noexcept;
        
        void initialize() override;
        int32_t main() override;
        void deinitialize() override;

        ThreadWorkerData data;

        Event thread_event;
        
        std::atomic<bool> b_running;
        std::atomic<bool> b_want_stop;

        void create_thread();
        void destroy_thread() noexcept;
        std::mutex thread_mutex;
        std::thread thread;
        std::atomic<bool> b_exist_thread;
    };
}
