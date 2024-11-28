/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"
#include <thread>

UDPClient::UDPClient(
    asio::io_context& io_context,
    unsigned short port,
    const std::string& server_ip,
    unsigned short server_port
):
    _socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
    _server_endpoint(asio::ip::address::from_string(server_ip), server_port),
    _packet_manager(io_context, _socket, Role::CLIENT)
{
    _packet_manager.start();
    // start_receive();
}

UDPClient::~UDPClient() { std::cout << "deleting UDPClient" << std::endl; }

void UDPClient::send_unreliable_packet(const std::string& message) {
    _packet_manager.send_unreliable_packet(message, _server_endpoint);
}

void UDPClient::send_reliable_packet(const std::string& message) {
    _packet_manager.send_reliable_packet(message, _server_endpoint);
}

std::string UDPClient::get_last_reliable_packet() {
    // std::string complete_message = _packet_manager.get_complete_message();
    // std::cout << "Received complete message: " << complete_message << std::endl;
    // return complete_message;
    return "";
}