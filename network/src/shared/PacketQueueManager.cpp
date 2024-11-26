/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PacketQueueManager.cpp
*/

#include "../../include/shared/PacketQueueManager.hpp"
#include <iostream>

void PacketQueueManager::enqueue(const packet& pkt, const asio::ip::udp::endpoint& endpoint) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.emplace(pkt, endpoint);
    }
    cv_.notify_one();
}

std::pair<packet, asio::ip::udp::endpoint> PacketQueueManager::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    auto item = queue_.front();
    queue_.pop();
    return item;
}

bool PacketQueueManager::empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

void PacketQueueManager::notify_all() { cv_.notify_all(); }

void PacketQueueManager::lock_and_wait(std::unique_lock<std::mutex>& lock, bool stop_processing) {
    // std::cout << "Waiting for packets..." << std::endl;
    cv_.wait(lock, [this, stop_processing] { return !queue_.empty() || stop_processing; });
}

void PacketQueueManager::manual_lock() { mutex_.lock(); }

void PacketQueueManager::manual_unlock() { mutex_.unlock(); }

std::size_t PacketQueueManager::get_size() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}