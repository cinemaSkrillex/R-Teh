/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#include "../../include/RtypeServer.hpp"

RtypeServer::RtypeServer(std::shared_ptr<UDPServer> server)
    : _server(server),
      _config("rtype_server/Config/server.config"),
      _game_instance(std::make_shared<GameInstance>()),
      _startTime(std::chrono::steady_clock::now()) {
    initCallbacks();
}

RtypeServer::~RtypeServer() {}