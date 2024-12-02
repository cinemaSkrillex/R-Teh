/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "UDPClient.hpp"

UDPClient::UDPClient(asio::io_context& io_context, unsigned short port,
                     const std::string& server_ip, unsigned short server_port)
    : _socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      _server_endpoint(asio::ip::address::from_string(server_ip), server_port),
      _packet_manager(io_context, _socket, Role::CLIENT) {
    _packet_manager.start();
    // start_receive();
}

UDPClient::~UDPClient() { std::cout << "deleting UDPClient" << std::endl; }

void UDPClient::send_new_client() {
    _packet_manager.send_new_client(_server_endpoint);
}

void UDPClient::send_test() {
    _packet_manager.send_test(_server_endpoint);
}

void UDPClient::send_unreliable_packet(const std::string& message) {
    _packet_manager.send_unreliable_packet(message, _server_endpoint);
}

void UDPClient::send_reliable_packet(const std::string& message) {
    _packet_manager.send_reliable_packet(message, _server_endpoint);
}

const std::string UDPClient::get_last_reliable_packet() {
    return _packet_manager.get_last_reliable_packet();
}

const std::string UDPClient::get_last_unreliable_packet() {
    return _packet_manager.get_last_unreliable_packet();
}