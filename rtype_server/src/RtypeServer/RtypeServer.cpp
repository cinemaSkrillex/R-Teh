/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

RtypeServer::RtypeServer(std::shared_ptr<UDPServer> server, bool server_vision)
    : _server(server),
      _game_instance(std::make_shared<GameInstance>(server_vision)),
      _server_config(),
      _startTime(std::chrono::steady_clock::now()),
      _scene_manager(_game_instance->getRegistryRef()) {
    initCallbacks();
    initEventHandlers();
    printServerStartupBanner();
    initScenes();
    _scene_manager.switchScene(SceneType::WAITING);
    updateScene();
}

RtypeServer::~RtypeServer() {}