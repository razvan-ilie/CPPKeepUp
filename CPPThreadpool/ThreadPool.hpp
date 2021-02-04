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

    unsigned int numThreads_;
    std::atomic_bool terminate_;
    bool stopped_;

    std::condition_variable condition_;
    std::vector<std::thread> pool_;
    std::deque<std::packaged_task<ReturnType()>> jobs_;
    std::mutex jobsMutex_;
};

#include "ThreadPool.cpp"

#endif