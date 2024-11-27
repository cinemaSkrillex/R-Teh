/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.cpp
*/

#include "../../include/Server/UDPServer.hpp"
#include <bits/this_thread_sleep.h>
#include <system_error>

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), client_endpoint_(),
      packet_manager_(io_context, socket_, Role::SERVER) {
    packet_manager_.start();
}

UDPServer::~UDPServer() { std::cout << "deleting UDPServer"; }

void UDPServer::setEndpoint(const asio::ip::udp::endpoint& endpoint) {
    client_endpoint_ = endpoint;
}

const asio::ip::udp::endpoint& UDPServer::getEndpoint() const {
    std::cout << "Client endpoint: " << client_endpoint_ << std::endl;
    return client_endpoint_;
}

const std::string UDPServer::getLastUnreliablePacket() {
    return packet_manager_.get_last_unreliable_packet();
}

/**
 * @brief Sends an unreliable packet to the specified endpoint.
 *
 * This function sends a packet containing the given message to the specified
 * endpoint using UDP. The packet is sent asynchronously and is considered
 * unreliable, meaning it is not critical if the packet is lost during
 * transmission (e.g., player position updates).
 *
 * @param message The message to be sent in the packet.
 * @param endpoint The destination endpoint to which the packet will be sent.
 */
void UDPServer::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& endpoint) {
    packet_manager_.send_unreliable_packet(message, endpoint);
}
/**
 * @brief Sends a reliable packet over UDP.
 *
 * This function sends a packet to the specified endpoint and ensures that the packet is reliably
 * delivered by implementing an acknowledgment mechanism. If the packet is not acknowledged within
 * a certain time frame, it will be resent.
 *
 * @param message The message to be sent.
 * @param endpoint The destination endpoint to which the message should be sent.
 */
void UDPServer::send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& endpoint) {
    packet_manager_.send_reliable_packet(message, endpoint);
}
