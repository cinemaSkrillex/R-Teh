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

#define BUFFER_SIZE 500
#define WINDOW_SIZE 5

#include "../Export.hpp"
#include <asio.hpp>
#include <iostream>
#include <array>
#include <unordered_set>

// Custom hash and equality for asio::ip::udp::endpoint
// we need it in order to know if we have already seen a client (kind of a select in C)
struct EndpointHash {
    std::size_t operator()(const asio::ip::udp::endpoint& endpoint) const {
        std::size_t h1 = std::hash<std::string>()(endpoint.address().to_string());
        std::size_t h2 = std::hash<unsigned short>()(endpoint.port());
        return h1 ^ (h2 << 1); // Combine the hashes
    }
};

struct EndpointEqual {
    bool operator()(const asio::ip::udp::endpoint& lhs, const asio::ip::udp::endpoint& rhs) const {
        return lhs.address() == rhs.address() && lhs.port() == rhs.port();
    }
};

struct packet {
    int               sequence_no;
    int               packet_size;
    std::vector<char> data;
};

// SERVER_API is a macro for the visibility of the class UDPServer,
// its for the shared library

class SERVER_API UDPServer {
  public:
    UDPServer(asio::io_context& io_context, unsigned short port);
    ~UDPServer();

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint);
    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint);
    const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>&
         get_known_clients() const;
    void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint);

  private:
    void start_receive();
    void handle_receive(std::size_t bytes_recvd);
    void handle_reliable_packet(const std::string& message, std::size_t colon_pos);
    void handle_unreliable_packet(const std::string& message);
    void handle_new_client(const asio::ip::udp::endpoint& client_endpoint);
    void handle_ack(const std::string& ack_message);

    void send_packet(const packet& pkt, const asio::ip::udp::endpoint& endpoint);
    void retransmit_unacknowledged_packets(const asio::ip::udp::endpoint& endpoint);

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
};

#endif // UDPSERVER_HPP