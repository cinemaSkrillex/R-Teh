/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"

UDPClient::UDPClient(asio::io_context& io_context, unsigned short port,
                     const std::string& server_ip, unsigned short server_port)
    : ANetwork<800>(io_context, port, Role::CLIENT) {
    _server_endpoint =
        asio::ip::udp::endpoint(asio::ip::address::from_string(server_ip), server_port);
}

// Callbacks
void UDPClient::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
    ANetwork<800>::setNewClientCallback(callback);  // Call the inherited method
}

// Send methods
void UDPClient::send_unreliable_packet(const std::array<char, 800>& message) {
    ANetwork<800>::send_unreliable_packet(message, _server_endpoint);  // Call the inherited method
}

void UDPClient::send_reliable_packet(const std::array<char, 800>& message) {
    ANetwork<800>::send_reliable_packet(message, _server_endpoint);  // Call the inherited method
}

// Receive methods
std::vector<std::array<char, 800>> UDPClient::get_unreliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::get_unreliable_messages_from_endpoint(
        endpoint);  // Call the inherited method
}

std::vector<std::array<char, 800>> UDPClient::get_reliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return ANetwork<800>::get_reliable_messages_from_endpoint(
        endpoint);  // Call the inherited method
}

std::array<char, 800> UDPClient::get_last_unreliable_packet_data() {
    return ANetwork<800>::get_last_unreliable_packet_data();  // Call the inherited method
}

std::array<char, 800> UDPClient::get_last_reliable_packet_data() {
    return ANetwork<800>::get_last_reliable_packet_data();  // Call the inherited method
}

// Miscellaneous methods
void UDPClient::send_new_client() {
    ANetwork<800>::send_new_client(_server_endpoint);  // Call the inherited method
}
