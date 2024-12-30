/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.cpp
*/

#include "../../include/Server/UDPServer.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <bits/this_thread_sleep.h>

#include <system_error>
#endif

#include "UDPServer.hpp"

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : ANetwork<1024>(io_context, port, Role::SERVER) {
    // You can add additional constructor logic here
}

void UDPServer::setEndpoint(const asio::ip::udp::endpoint& endpoint) {
    _client_endpoint = endpoint;
}

const asio::ip::udp::endpoint& UDPServer::getEndpoint() const { return _client_endpoint; }

// Callbacks
void UDPServer::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
    _new_client_callback                 = callback;
    _packet_manager._new_client_callback = callback;
}

// Send methods
void UDPServer::send_unreliable_packet(const std::array<char, 1024>&  message,
                                       const asio::ip::udp::endpoint& endpoint) {
    ANetwork<1024>::send_unreliable_packet(message, endpoint);  // Call the inherited method
}

void UDPServer::send_reliable_packet(const std::array<char, 1024>&  message,
                                     const asio::ip::udp::endpoint& endpoint) {
    ANetwork<1024>::send_reliable_packet(message, endpoint);  // Call the inherited method
}

// Receive methods
std::vector<std::array<char, 1024>> UDPServer::get_unreliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<1024>::get_unreliable_messages_from_endpoint(
        endpoint);  // Call the inherited method
}

std::vector<std::array<char, 1024>> UDPServer::get_reliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<1024>::get_reliable_messages_from_endpoint(
        endpoint);  // Call the inherited method
}

std::array<char, 1024> UDPServer::get_last_unreliable_packet_data() {
    return ANetwork<1024>::get_last_unreliable_packet_data();  // Call the inherited method
}

std::array<char, 1024> UDPServer::get_last_reliable_packet_data() {
    return ANetwork<1024>::get_last_reliable_packet_data();  // Call the inherited method
}

// Miscellaneous methods
void UDPServer::send_new_client(const asio::ip::udp::endpoint& endpoint) {
    ANetwork<1024>::send_new_client(endpoint);  // Call the inherited method
}
