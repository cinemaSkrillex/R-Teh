/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPClient.cpp
*/

#include "TCPClient.hpp"

TCPClient::TCPClient(const std::string &host, unsigned short port) : _packet_manager(Role::CLIENT) {
    _packet_manager.start_client(host, port);
}