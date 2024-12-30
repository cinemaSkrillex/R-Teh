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

#include "../shared/PacketManager.hpp"
#include "ANetwork.hpp"

class UDPClient : public ANetwork<1024> {
   public:
    // Constructor
    UDPClient(asio::io_context& io_context, unsigned short port, const std::string& server_ip,
              unsigned short server_port);

    // Callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback);

    // Send methods
    void send_unreliable_packet(const std::array<char, 1024>& message);
    void send_reliable_packet(const std::array<char, 1024>& message);

    // Receive methods
    std::vector<std::array<char, 1024>> get_unreliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::vector<std::array<char, 1024>> get_reliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::array<char, 1024> get_last_unreliable_packet_data();
    std::array<char, 1024> get_last_reliable_packet_data();

    // Miscellaneous methods
    void send_new_client();

   private:
    asio::ip::udp::endpoint _client_endpoint;  // The client's endpoint
};

#endif  // UDPCLIENT_HPP
