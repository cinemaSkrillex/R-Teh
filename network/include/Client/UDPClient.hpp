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

class UDPClient {
  public:
    UDPClient(asio::io_context& io_context, unsigned short port);

    void send_ack(const asio::ip::udp::endpoint& server_endpoint, std::uint32_t sequence_number);
    void receive_packet();

  private:
    void handle_reliable_packet(const std::string& message, std::size_t colon_pos);
    void handle_unreliable_packet(const std::string& message);

    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint server_endpoint_;
    std::array<char, 1024>  recv_buffer_;
};

#endif // UDPCLIENT_HPP