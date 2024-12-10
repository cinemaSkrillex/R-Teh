/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#include "../include/RtypeServer.hpp"

RtypeServer::RtypeServer(std::shared_ptr<UDPServer> server) : _server(server), _startTime(std::chrono::steady_clock::now()) {}

RtypeServer::~RtypeServer() {}