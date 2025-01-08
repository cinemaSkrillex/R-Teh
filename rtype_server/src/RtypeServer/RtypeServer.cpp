/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void preloadTextures() {}

RtypeServer::RtypeServer(std::shared_ptr<UDPServer> server)
    : _server(server),
      _game_instance(std::make_shared<GameInstance>()),
      _server_config(),
      _startTime(std::chrono::steady_clock::now()),
      _server_map(std::make_shared<ServerMap>()) {
    initCallbacks();
    _server_map->loadFromJSON("../../assets/maps/map.json");
}

RtypeServer::~RtypeServer() {}