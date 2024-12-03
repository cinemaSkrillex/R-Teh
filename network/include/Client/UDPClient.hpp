/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.hpp
*/

#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "../shared/PacketManager.hpp"

class UDPClient {
   public:
    UDPClient(asio::io_context& io_context, unsigned short port, const std::string& server_ip,
              unsigned short server_port);
    ~UDPClient();

    void              send_unreliable_packet(const std::string& message);
    void              send_reliable_packet(const std::string& message);
    void              send_new_client();
    void              send_test();
    const std::string get_last_reliable_packet();
    const std::string get_last_unreliable_packet();

   private:
    asio::ip::udp::endpoint _server_endpoint;
    asio::ip::udp::socket   _socket;
    asio::io_context&       _io_context;
    std::thread             _io_context_thread;
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;

    // Reliable packet handling via sequence numbers
    PacketManager _packet_manager;
    // Packet processing
};

#endif  // UDPCLIENT_HPP