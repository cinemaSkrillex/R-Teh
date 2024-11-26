/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ThreadPool.cpp
*/

#include "../../include/shared/ThreadPool.hpp"

/**
 * @brief Worker thread function that executes tasks from the task queue.
 *
 * This function is responsible for executing tasks from the task queue in a separate thread.
 * It continuously waits for new tasks to be added to the queue and executes them one by one.
 * If the thread pool is stopped and there are no more tasks in the queue, the thread will exit.
 */
void ThreadPool::workerThread() {
    while (true) {
        std::function<void()>        task;
        std::unique_lock<std::mutex> lock(_queue_mutex);
        _cv.wait(lock, [this] { return !_tasks.empty() || _stop; });
        if (_stop && _tasks.empty()) {
            return;
        }
        task = std::move(_tasks.front());
        _tasks.pop();
        lock.unlock();
        task();
    }
}

/**
 * @brief Constructs a ThreadPool object with the specified number of threads.
 *
 * @param num_threads The number of threads to create in the thread pool.
 */
ThreadPool::ThreadPool(size_t num_threads) {
    for (size_t i = 0; i < num_threads; i++) {
        _threads.emplace_back([this] { workerThread(); });
    }
}

/**
 * @brief Destructor for the ThreadPool class.
 *
 * This destructor stops the ThreadPool by setting the _stop flag to true,
 * notifies all waiting threads using the condition variable _cv, and joins
 * all the worker threads to wait for their completion.
 */
ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(_queue_mutex);
    _stop = true;
    lock.unlock();

    _cv.notify_all();

    for (std::thread& thread : _threads)
        thread.join();
}

/**
 * @brief Enqueues a task to be executed by the thread pool.
 *
 * @param task The task to be executed.
 */
void ThreadPool::enqueue(std::function<void()> task) {
    std::unique_lock<std::mutex> lock(_queue_mutex);
    _tasks.emplace(std::move(task));
    lock.unlock();
    _cv.notify_one();
}