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

class UDPServer : public ANetwork<1024> {
   public:
    // Constructor
    UDPServer(asio::io_context& io_context, unsigned short port);

    // Set the endpoint for the client
    void                           setEndpoint(const asio::ip::udp::endpoint& endpoint);
    const asio::ip::udp::endpoint& getEndpoint() const;

    // Callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback);

    // Send methods
    void send_unreliable_packet(const std::array<char, 1024>&  message,
                                const asio::ip::udp::endpoint& endpoint);
    void send_reliable_packet(const std::array<char, 1024>&  message,
                              const asio::ip::udp::endpoint& endpoint);

    // Receive methods
    std::vector<std::array<char, 1024>> get_unreliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::vector<std::array<char, 1024>> get_reliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::array<char, 1024> get_last_unreliable_packet_data();
    std::array<char, 1024> get_last_reliable_packet_data();

    // Miscellaneous methods
    void send_new_client(const asio::ip::udp::endpoint& endpoint);
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getClients();

   private:
    asio::ip::udp::endpoint _client_endpoint;  // The client's endpoint
};

#endif  // UDPSERVER_HPP