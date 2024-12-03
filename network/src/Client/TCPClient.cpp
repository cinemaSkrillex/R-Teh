/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPClient.cpp
*/

#include "../../include/Client/TCPClient.hpp"

TCPClient::TCPClient(const std::string& host, unsigned short port)
    : _packet_manager(std::make_shared<TCPPacketManager>(Role::CLIENT)) {
    _packet_manager->start_client(host, port);
}

TCPClient::~TCPClient() {
    std::cout << "closing packet manager" << std::endl;
    _packet_manager->close();
}

void TCPClient::send_message(const std::string& message) {
    _packet_manager->client_send_message(message);
}