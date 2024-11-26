/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PacketManager.cpp
*/

#include "../../include/shared/PacketManager.hpp"

PacketManager::PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role)
    : sequence_number_(0), retransmission_timer_(io_context), socket_(socket), role_(role),
      stop_processing_(false), work_guard_(asio::make_work_guard(io_context)) {}

PacketManager::~PacketManager() {
    stop_processing_ = true;
    queue_cv_.notify_all();
    if (receive_packet_thread_.joinable())
        receive_packet_thread_.join();
    if (process_packet_thread_.joinable())
        process_packet_thread_.join();
    if (send_packet_thread_.joinable())
        send_packet_thread_.join();
    if (retransmission_thread_.joinable())
        retransmission_thread_.join();
}

void PacketManager::start() {
    receive_packet_thread_ = std::thread(&PacketManager::start_receive, this);
    process_packet_thread_ = std::thread(&PacketManager::process_packets, this);
    send_packet_thread_    = std::thread(&PacketManager::send_packets, this);
    retransmission_thread_ = std::thread(&PacketManager::handle_retransmissions, this);
}
