/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PacketQueueManager.hpp
*/

#ifndef PACKETQUEUEMANAGER_HPP
#define PACKETQUEUEMANAGER_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <asio.hpp>
#include <iostream>
#include "PacketUtils.hpp"

class PacketQueueManager {
  public:
    ~PacketQueueManager() { std::cout << "deleting PacketQueueManager" << std::endl; }
    void enqueue(const packet& pkt, const asio::ip::udp::endpoint& endpoint);
    std::pair<packet, asio::ip::udp::endpoint> dequeue();
    bool                                       empty();
    void                                       notify_all();
    void lock_and_wait(std::unique_lock<std::mutex>& lock, bool stop_processin);
    void manual_lock();
    void manual_unlock();
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.clear();
    }
    void stop_retransmissions() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            stop_retransmissions_ = true;
        }
        cv_.notify_all();
    }
    std::mutex& get_mutex() { return mutex_; }
    std::mutex& get_queue_mutex() { return queue_mutex_; }
    std::deque<std::pair<packet, asio::ip::udp::endpoint>> get_queue() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_;
    }
    // get the size of the queue
    std::size_t              get_size();
    std::condition_variable& get_cv() { return cv_; }

    bool stop_retransmissions_ = false;

  private:
    std::deque<std::pair<packet, asio::ip::udp::endpoint>> queue_;
    std::mutex                                             queue_mutex_;
    std::mutex                                             mutex_;
    std::condition_variable                                cv_;
};

#endif // PACKETQUEUEMANAGER_HPP