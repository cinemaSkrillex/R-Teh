/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.cpp
*/

#include "../../include/Server/TCPServer.hpp"

TCPServer::TCPServer(unsigned short port) : _packet_manager(Role::SERVER) {
    _packet_manager.start_server(port);
}
