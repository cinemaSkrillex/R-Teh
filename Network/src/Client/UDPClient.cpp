/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"

UDPClient::UDPClient(asio::io_context& ioContext, unsigned short port, const std::string& serverIp,
                     unsigned short serverPort)
    : ANetwork<800>(ioContext, port, Role::CLIENT) {
    _serverEndpoint = asio::ip::udp::endpoint(asio::ip::address::from_string(serverIp), serverPort);
}

// Callbacks
void UDPClient::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
    ANetwork<800>::setNewClientCallback(callback);
}

// Send methods
void UDPClient::sendUnreliablePacket(const std::array<char, 800>& message) {
    ANetwork<800>::sendUnreliablePacket(message, _serverEndpoint);
}

void UDPClient::sendReliablePacket(const std::array<char, 800>& message) {
    ANetwork<800>::sendReliablePacket(message, _serverEndpoint);
}

// Receive methods
std::vector<std::array<char, 800>> UDPClient::getUnreliableMessagesFromEndpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::getUnreliableMessagesFromEndpoint(endpoint);
}

std::vector<std::array<char, 800>> UDPClient::getReliableMessagesFromEndpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::getReliableMessagesFromEndpoint(endpoint);
}

std::array<char, 800> UDPClient::getLastUnreliablePacketData() {
    return ANetwork<800>::getLastUnreliablePacketData();
}

std::array<char, 800> UDPClient::getLastReliablePacketData() {
    return ANetwork<800>::getLastReliablePacketData();
}

// Miscellaneous methods
void UDPClient::sendNewClient() { ANetwork<800>::sendNewClient(_serverEndpoint); }
