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

#include <array>
#include <asio.hpp>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "ANetwork.hpp"

class UDPServer : public ANetwork<1024> {  // 1024 is the buffer size
   public:
    // Constructor
    UDPServer(asio::io_context& io_context, unsigned short port)
        : ANetwork(io_context, port, Role::SERVER) {}

    // Set endpoint for the client
    void setEndpoint(const asio::ip::udp::endpoint& endpoint) { _client_endpoint = endpoint; }

    const asio::ip::udp::endpoint& getEndpoint() const { return _client_endpoint; }

    // Get list of known clients
    const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getClients() {
        return _packet_manager.getKnownClients();
    }

    // Send reliable and unreliable packets (delegates to ANetwork's PacketManager)
    void send_unreliable_packet(const std::array<char, 1024>&  message,
                                const asio::ip::udp::endpoint& endpoint) {
        ANetwork<1024>::send_unreliable_packet(message, endpoint);  // Call the inherited method
    }

    void send_reliable_packet(const std::array<char, 1024>&  message,
                              const asio::ip::udp::endpoint& endpoint) {
        ANetwork<1024>::send_reliable_packet(message, endpoint);  // Call the inherited method
    }

   private:
    asio::ip::udp::endpoint _client_endpoint;  // Stores the client's endpoint
};

#endif  // UDPSERVER_HPP