/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.hpp
*/

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#include "../shared/PacketUtils.hpp"
#include "../shared/PacketManager.hpp"

#include <asio.hpp>
#include <iostream>
#include <array>
#include <unordered_set>

// SERVER_API is a macro for the visibility of the class UDPServer,
// its for the shared library

class UDPServer {
  public:
    UDPServer(asio::io_context& io_context, unsigned short port);
    ~UDPServer();

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint);
    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint);
    const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>&
    get_known_clients() const;

  private:
    void start_receive();
    void handle_receive(std::size_t bytes_recvd);
    void handle_reliable_packet(const std::string& message, int sequence_number);
    void handle_unreliable_packet(const std::string& message);
    void handle_new_client(const asio::ip::udp::endpoint& client_endpoint);
    void handle_ack(const std::string& ack_message);

    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1024>  recv_buffer_;

    // Reliable packet handling via sequence numbers
    std::unordered_map<int, packet> unacknowledged_packets_;
    std::uint32_t                   sequence_number_ = 0;
    // retransmission timer
    asio::steady_timer retransmission_timer_;
    // Track known clients
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> known_clients_;
    PacketManager                                                            packet_manager_;
};

#endif // UDPSERVER_HPP