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
    UDPClient(asio::io_context& io_context, unsigned short port);
    ~UDPClient();

    void        send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& server_endpoint);
    void        send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& server_endpoint);
    std::string get_last_reliable_packet();

  private:
    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint server_endpoint_;

    // Reliable packet handling via sequence numbers
    PacketManager packet_manager_;
    // Packet processing
};

#endif // UDPCLIENT_HPP