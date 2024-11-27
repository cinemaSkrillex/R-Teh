/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ThreadPool.hpp
*/

#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
  public:
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency());

    ~ThreadPool();
    void                              enqueue(std::function<void()> task);
    void                              workerThread();
    std::queue<std::function<void()>> _tasks;

  private:
    std::vector<std::thread> _threads;
    std::mutex               _queue_mutex;
    std::condition_variable  _cv;
    bool                     _stop = false;
};