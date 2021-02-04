// Generic threadpool implementation that can take functions
// which return a given type and return a future of that type

#include <iostream>
#include <set>
#include <random>
#include <functional>

#include "ThreadPool.hpp"

std::size_t sizeOfSortedSetWithReplacement(long N, long maxNum) {
    std::set<long> res;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, maxNum);

    for (int i = 0; i < N; i++) res.insert(dist(mt));
    return res.size();
}

int main()
{
    // Threadpool of tasks that return future<std::size_t>
    ThreadPool<std::size_t> pool;

    int numIter = 12;

    std::vector<std::future<std::size_t>> v;
    for (int i = 0; i < numIter; i++) {
        auto func = std::bind(sizeOfSortedSetWithReplacement, 100000 * (i % 6 + 1), 100000 * (i % 6 + 1));
        auto fut = pool.AddTask(func);
        v.push_back(std::move(fut));
    }

    std::cout << "All tasks submitted\n";

    for (int i = 0; i < numIter; i++) std::cout << v[i].get() << "\n";

    return 0;
}
