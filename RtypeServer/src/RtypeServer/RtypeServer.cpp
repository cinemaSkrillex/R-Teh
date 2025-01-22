/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

RtypeServer::RtypeServer(std::shared_ptr<UDPServer> server, bool server_vision)
    : _server(server),
      _gameInstance(std::make_shared<GameInstance>(server_vision)),
      _serverConfig(),
      _startTime(std::chrono::steady_clock::now()),
      _sceneManager(),
      _log(std::make_shared<Log>("score.log")) {
    initCallbacks();
    initEventHandlers();
    printServerStartupBanner();
    initScenes();
}

RtypeServer::~RtypeServer() {}