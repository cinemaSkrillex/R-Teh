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

#include "../shared/PacketManager.hpp"

class UDPServer {
   public:
    UDPServer(asio::io_context& io_context, unsigned short port);
    ~UDPServer();

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint);
    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint);

    void setEndpoint(const asio::ip::udp::endpoint& endpoint);
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint& client_endpoint)>& callback);

    const asio::ip::udp::endpoint& getEndpoint() const;
    const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getClients();
    const std::string getLastUnreliablePacket();
    const std::string getLastUnreliablePacket(const asio::ip::udp::endpoint& endpoint);

   private:
    asio::ip::udp::socket   _socket;
    asio::ip::udp::endpoint _client_endpoint;

    PacketManager _packet_manager;
};

#endif  // UDPSERVER_HPP