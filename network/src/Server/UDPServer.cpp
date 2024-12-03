/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.cpp
*/

#include "UDPServer.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <bits/this_thread_sleep.h>

#include <system_error>
#endif

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : _socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      _client_endpoint(),
      _packet_manager(io_context, _socket, Role::SERVER),
      _io_context(io_context),
      _work_guard(asio::make_work_guard(io_context)) {
    _packet_manager.start();
    _io_context_thread = std::thread([this]() { _io_context.run(); });
}

UDPServer::~UDPServer() {
    std::cout << "deleting UDPServer";
    _io_context.stop();
    if (_io_context_thread.joinable()) {
        _io_context_thread.join();
    }
}

void UDPServer::setEndpoint(const asio::ip::udp::endpoint& endpoint) {
    _client_endpoint = endpoint;
}

void UDPServer::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint& client_endpoint)>& callback) {
    _packet_manager._new_client_callback = callback;
}

const asio::ip::udp::endpoint& UDPServer::getEndpoint() const {
    return _client_endpoint;
}

const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>
UDPServer::getClients() {
    return _packet_manager.getKnownClients();
}

const std::string UDPServer::getLastReliablePacket() {
    return _packet_manager.get_last_reliable_packet();
}

const std::string UDPServer::getLastUnreliablePacket() {
    return _packet_manager.get_last_unreliable_packet();
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
    _packet_manager.send_unreliable_packet(message, endpoint);
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
    _packet_manager.send_reliable_packet(message, endpoint);
}
