/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPClient.cpp
*/

#include "../../include/Client/TCPClient.hpp"
#include <iostream>

TCPClient::TCPClient(const std::string& host, unsigned short port)
    : _packet_manager(std::make_shared<TCPPacketManager>(Role::CLIENT)) {
    _packet_manager->start_client(host, port);
}

TCPClient::~TCPClient() {
    _packet_manager->close();
}

void TCPClient::send_message(const std::string& message) {
    _packet_manager->send_message_to_server(message);
}
