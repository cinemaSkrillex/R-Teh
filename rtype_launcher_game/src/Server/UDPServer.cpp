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

#include "../../include/Server/UDPServer.hpp"

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : ANetwork<800>(io_context, port, Role::SERVER) {}

void UDPServer::setEndpoint(const asio::ip::udp::endpoint& endpoint) {
    _client_endpoint = endpoint;
}

const asio::ip::udp::endpoint& UDPServer::getEndpoint() const { return _client_endpoint; }

asio::ip::udp::endpoint UDPServer::getLocalEndpoint() const {
    return ANetwork<800>::getLocalEndpoint();
}

// Callbacks
void UDPServer::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
    ANetwork<800>::setNewClientCallback(callback);
}

// Send methods
void UDPServer::send_unreliable_packet(const std::array<char, 800>&   message,
                                       const asio::ip::udp::endpoint& endpoint) {
    ANetwork<800>::send_unreliable_packet(message, endpoint);
}

void UDPServer::send_reliable_packet(const std::array<char, 800>&   message,
                                     const asio::ip::udp::endpoint& endpoint) {
    ANetwork<800>::send_reliable_packet(message, endpoint);
}

// Receive methods
std::vector<std::array<char, 800>> UDPServer::get_unreliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::get_unreliable_messages_from_endpoint(endpoint);
}

std::vector<std::array<char, 800>> UDPServer::get_reliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::get_reliable_messages_from_endpoint(endpoint);
}

std::array<char, 800> UDPServer::get_last_unreliable_packet_data() {
    return ANetwork<800>::get_last_unreliable_packet_data();
}

std::array<char, 800> UDPServer::get_last_reliable_packet_data() {
    return ANetwork<800>::get_last_reliable_packet_data();
}

// Miscellaneous methods
void UDPServer::send_new_client(const asio::ip::udp::endpoint& endpoint) {
    ANetwork<800>::send_new_client(endpoint);
}

std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> UDPServer::getClients() {
    return ANetwork<800>::getClients();
}
