#ifndef THREADPOOL_CPP
#define THREADPOOL_CPP

#include "ThreadPool.hpp"

template<typename ReturnType>
ThreadPool<ReturnType>::ThreadPool() : 
	numThreads_(std::thread::hardware_concurrency() - 1), // leave last one for main thread!
	terminate_(false),
	stopped_(false)
{
    for (int i = 0; i < numThreads_; i++) pool_.push_back(std::thread(&ThreadPool::Run, this));
}

template<typename ReturnType>
ThreadPool<ReturnType>::~ThreadPool() {
    if (!stopped_) Shutdown();
}

template<typename ReturnType>
std::future<ReturnType> ThreadPool<ReturnType>::AddTask(std::function<ReturnType()> task) {
    std::packaged_task<ReturnType()> packagedTask(task);
    std::future<ReturnType> fut = packagedTask.get_future();
    {
        std::unique_lock<std::mutex> lock(jobsMutex_);
        jobs_.push_back(std::move(packagedTask));
    }
    condition_.notify_one();
    return fut;
}

template<typename ReturnType>
void ThreadPool<ReturnType>::Shutdown() {
    terminate_ = true; // atomic
    condition_.notify_all();
    for (std::thread& th : pool_) th.join();
    pool_.clear();
    stopped_ = true;
}

template<typename ReturnType>
void ThreadPool<ReturnType>::Run() {
    std::packaged_task<ReturnType()> job;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(jobsMutex_);
            condition_.wait(lock, [&] { return !jobs_.empty() || terminate_.load(); });
            if (!jobs_.empty()) {
                job = std::move(jobs_.front());
                jobs_.pop_front();
            }
            else if (terminate_.load())
                break;
        }
        job();
    }
}

#endif
