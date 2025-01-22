/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.hpp
*/

#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "ANetwork.hpp"

class UDPClient : public ANetwork<800> {
   public:
    // Constructor
    UDPClient(asio::io_context& ioContext, unsigned short port, const std::string& serverIp,
              unsigned short serverPort);

    // Callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback);

    // Send methods
    void sendUnreliablePacket(const std::array<char, 800>& message);
    void sendReliablePacket(const std::array<char, 800>& message);

    // Receive methods
    std::vector<std::array<char, 800>> getUnreliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::vector<std::array<char, 800>> getReliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::array<char, 800> getLastUnreliablePacketData();
    std::array<char, 800> getLastReliablePacketData();

    // Miscellaneous methods
    void sendNewClient();

   private:
    asio::ip::udp::endpoint _serverEndpoint;
};

#endif  // UDPCLIENT_HPP
