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

UDPServer::UDPServer(asio::io_context& ioContext, unsigned short port)
    : ANetwork<800>(ioContext, port, Role::SERVER) {}

void UDPServer::setEndpoint(const asio::ip::udp::endpoint& endpoint) { _clientEndpoint = endpoint; }

const asio::ip::udp::endpoint& UDPServer::getEndpoint() const { return _clientEndpoint; }

asio::ip::udp::endpoint UDPServer::getLocalEndpoint() const {
    return ANetwork<800>::getLocalEndpoint();
}

// Callbacks
void UDPServer::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
    ANetwork<800>::setNewClientCallback(callback);
}

// Send methods
void UDPServer::sendUnreliablePacket(const std::array<char, 800>&   message,
                                     const asio::ip::udp::endpoint& endpoint) {
    ANetwork<800>::sendUnreliablePacket(message, endpoint);
}

void UDPServer::sendReliablePacket(const std::array<char, 800>&   message,
                                   const asio::ip::udp::endpoint& endpoint) {
    ANetwork<800>::sendReliablePacket(message, endpoint);
}

// Receive methods
std::vector<std::array<char, 800>> UDPServer::getUnreliableMessagesFromEndpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::getUnreliableMessagesFromEndpoint(endpoint);
}

std::vector<std::array<char, 800>> UDPServer::getReliableMessagesFromEndpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::getReliableMessagesFromEndpoint(endpoint);
}

std::array<char, 800> UDPServer::getLastUnreliablePacketData() {
    return ANetwork<800>::getLastUnreliablePacketData();
}

std::array<char, 800> UDPServer::getLastReliablePacketData() {
    return ANetwork<800>::getLastReliablePacketData();
}

// Miscellaneous methods
void UDPServer::sendNewClient(const asio::ip::udp::endpoint& endpoint) {
    ANetwork<800>::sendNewClient(endpoint);
}

std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> UDPServer::getClients() {
    return ANetwork<800>::getClients();
}
