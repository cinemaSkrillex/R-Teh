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
#include "../shared/PacketUtils.hpp"
#include "../shared/PacketManager.hpp"

class CLIENT_API UDPClient {
  public:
    UDPClient(asio::io_context& io_context, unsigned short port);

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& server_endpoint);
    void send_reliable_packet(const std::string&             message,
                              const asio::ip::udp::endpoint& server_endpoint);
    void start_receive();

  private:
    void send_ack(std::uint32_t sequence_number);
    void handle_reliable_packet(const packet& pkt);
    void handle_unreliable_packet(const std::string& message);
    void handle_ack(const std::string& ack_message);
    void handle_receive(std::size_t bytes_recvd);

    void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint);
    void retransmit_unacknowledged_packets(const asio::ip::udp::endpoint& endpoint);
    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint server_endpoint_;
    std::array<char, 1024>  recv_buffer_;

    // Reliable packet handling via sequence numbers
    std::unordered_map<int, packet> unacknowledged_packets_;
    std::uint32_t                   sequence_number_ = 0;
    // retransmission timer
    asio::steady_timer retransmission_timer_;

    PacketManager packet_manager_;
};

#endif // UDPCLIENT_HPP