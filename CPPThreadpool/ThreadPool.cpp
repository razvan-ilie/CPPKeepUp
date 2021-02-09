#ifndef THREADPOOL_CPP
#define THREADPOOL_CPP

#include "ThreadPool.hpp"

template<typename ReturnType>
ThreadPool<ReturnType>::ThreadPool() : 
    m_numThreads(std::thread::hardware_concurrency() - 1), // leave last one for main thread!
    m_terminate(false),
    m_stopped(false)
{
    for (int i = 0; i < m_numThreads; i++) m_pool.push_back(std::thread(&ThreadPool::Run, this));
}

template<typename ReturnType>
ThreadPool<ReturnType>::~ThreadPool() {
    if (!m_stopped) Shutdown();
}

template<typename ReturnType>
std::future<ReturnType> ThreadPool<ReturnType>::AddTask(std::function<ReturnType()> task) {
    std::packaged_task<ReturnType()> packagedTask(task);
    std::future<ReturnType> fut = packagedTask.get_future();
    {
        std::unique_lock<std::mutex> lock(m_jobsMutex);
        m_jobs.push_back(std::move(packagedTask));
    }
    m_condition.notify_one();
    return fut;
}

template<typename ReturnType>
void ThreadPool<ReturnType>::Shutdown() {
    m_terminate = true; // atomic
    m_condition.notify_all();
    for (std::thread& th : m_pool) th.join();
    m_pool.clear();
    m_stopped = true;
}

template<typename ReturnType>
void ThreadPool<ReturnType>::Run() {
    std::packaged_task<ReturnType()> job;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(m_jobsMutex);
            m_condition.wait(lock, [&] { return !m_jobs.empty() || m_terminate.load(); });
            if (!m_jobs.empty()) {
                job = std::move(m_jobs.front());
                m_jobs.pop_front();
            }
            else if (m_terminate.load())
                break;
        }
        job();
    }
}

#endif
