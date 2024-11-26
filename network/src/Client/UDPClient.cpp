/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"
#include <thread>

UDPClient::UDPClient(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), server_endpoint_(),
      packet_manager_(io_context, socket_, Role::CLIENT) {
    packet_manager_.start();
    // start_receive();
}

UDPClient::~UDPClient() { std::cout << "deleting UDPClient" << std::endl; }

void UDPClient::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& server_endpoint) {
    packet_manager_.send_unreliable_packet(message, server_endpoint);
}

void UDPClient::send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& endpoint) {
    packet_manager_.send_reliable_packet(message, endpoint);
}