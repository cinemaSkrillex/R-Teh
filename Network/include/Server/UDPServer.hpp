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

#include "../Shared/ANetwork.hpp"

class UDPServer : public ANetwork<800> {
   public:
    // Constructor
    UDPServer(asio::io_context& ioContext, unsigned short port);

    // Set the endpoint for the client
    void                           setEndpoint(const asio::ip::udp::endpoint& endpoint);
    const asio::ip::udp::endpoint& getEndpoint() const;
    asio::ip::udp::endpoint        getLocalEndpoint() const;

    // Callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback);

    // Send methods
    void sendUnreliablePacket(const std::array<char, 800>&   message,
                              const asio::ip::udp::endpoint& endpoint);
    void sendReliablePacket(const std::array<char, 800>&   message,
                            const asio::ip::udp::endpoint& endpoint);

    // Receive methods
    std::vector<std::array<char, 800>> getUnreliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::vector<std::array<char, 800>> getReliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::array<char, 800> getLastUnreliablePacketData();
    std::array<char, 800> getLastReliablePacketData();

    // Miscellaneous methods
    void sendNewClient(const asio::ip::udp::endpoint& endpoint);
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getClients();

   private:
    asio::ip::udp::endpoint _clientEndpoint;  // The client's endpoint
};

#endif  // UDPSERVER_HPP