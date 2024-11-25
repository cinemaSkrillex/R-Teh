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
#include "../ClientExport.hpp"

#define BUFFER_SIZE 500

struct packet {
    int  sequence_no;
    int  packet_size;
    char data[BUFFER_SIZE];
};

class CLIENT_API UDPClient {
  public:
    UDPClient(asio::io_context& io_context, unsigned short port);

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& server_endpoint);
    void send_reliable_packet(const std::string&             message,
                              const asio::ip::udp::endpoint& server_endpoint);

    void start_receive();
    void send_ack(std::uint32_t sequence_number);

  private:
    void handle_reliable_packet(const packet& pkt);
    void handle_unreliable_packet(const std::string& message);
    void handle_ack(std::uint32_t sequence_number);

    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint server_endpoint_;
    std::array<char, 1024>  recv_buffer_;

    std::unordered_map<std::uint32_t, packet> unacknowledged_packets_;
    std::uint32_t                             sequence_number_ = 0;
};

#endif // UDPCLIENT_HPP