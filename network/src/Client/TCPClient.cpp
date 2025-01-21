/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPClient.cpp
*/

#include "../../include/Client/TCPClient.hpp"

#include <iostream>

TCPClient::TCPClient(const std::string& host, unsigned short port)
    : _packetManager(std::make_shared<TCPPacketManager>(Role::CLIENT)) {
    _packetManager->startClient(host, port);
}

TCPClient::~TCPClient() { _packetManager->close(); }

void TCPClient::sendMessage(const std::string& message) {
    _packetManager->sendMessageToServer(message);
}