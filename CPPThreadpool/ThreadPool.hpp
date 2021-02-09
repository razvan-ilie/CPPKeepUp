#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <future>
#include <deque>
#include <atomic>
#include <vector>

template <typename ReturnType>
class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = default;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = default;

    std::future<ReturnType> AddTask(std::function<ReturnType()>);
    void Shutdown();

private:
    void Run();

    unsigned int m_numThreads;
    std::atomic_bool m_terminate;
    bool m_stopped;

    std::condition_variable m_condition;
    std::vector<std::thread> m_pool;
    std::deque<std::packaged_task<ReturnType()>> m_jobs;
    std::mutex m_jobsMutex;
};

#include "ThreadPool.cpp"

#endif