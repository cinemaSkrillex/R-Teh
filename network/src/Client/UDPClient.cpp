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
      _packet_manager(io_context, _socket, Role::CLIENT),
      _io_context(io_context),
      _work_guard(asio::make_work_guard(io_context)) {
    _packet_manager.start();
    _io_context_thread = std::thread([this]() { _io_context.run(); });
}

UDPClient::~UDPClient() {
    _io_context.stop();
    if (_io_context_thread.joinable()) {
        _io_context_thread.join();
    }
}

void UDPClient::send_new_client() { _packet_manager.send_new_client(_server_endpoint); }

void UDPClient::send_test() { _packet_manager.send_test(_server_endpoint); }

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

std::vector<std::string> UDPClient::get_unreliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return _packet_manager.get_unreliable_messages_from_endpoint(endpoint);
}

std::vector<std::string> UDPClient::get_reliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return _packet_manager.get_reliable_messages_from_endpoint(endpoint);
}