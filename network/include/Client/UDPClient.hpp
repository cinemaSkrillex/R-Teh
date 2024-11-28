/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.hpp
*/

#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <asio.hpp>
#include <iostream>
#include <array>
#include <string>
#include <map>
#include <queue>
#include "../shared/PacketManager.hpp"

class UDPClient {
  public:
    UDPClient(asio::io_context& io_context,
              unsigned short port,
              const std::string& server_ip,
              unsigned short server_port
    );
    ~UDPClient();

    void        send_unreliable_packet(const std::string& message);
    void        send_reliable_packet(const std::string& message);
    void        send_new_client();
    void        send_test();
    std::string get_last_reliable_packet();

  private:
    asio::ip::udp::socket   _socket;
    asio::ip::udp::endpoint _server_endpoint;

    // Reliable packet handling via sequence numbers
    PacketManager           _packet_manager;
    // Packet processing
};

#endif // UDPCLIENT_HPP