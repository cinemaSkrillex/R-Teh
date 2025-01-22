/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MenuScene
*/

#ifndef MENUSCENE_HPP_
#define MENUSCENE_HPP_

#include <iostream>
#include <memory>

#include "../../Game/GameInstance.hpp"
#include "MapInitializer.hpp"
#include "PlayerInitializer.hpp"
#include "Scene.hpp"
#include "ServerConfig.hpp"

class MenuScene : public Scene {
   public:
    MenuScene(std::shared_ptr<GameInstance> gameInstance, RtypeServer* server,
              ServerConfig& serverConfig, std::shared_ptr<UDPServer> udpServer)
        : _gameInstance(gameInstance),
          _server(server),
          _serverConfig(serverConfig),
          _UdpServer(udpServer) {}

    void initialize(RealEngine::Registry& registry) override {
        std::cout << "Initializing MENU scene" << std::endl;
    }

    void update(float deltaTime) override {}

    void render() override {}

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    RtypeServer*                  _server;
    ServerConfig                  _serverConfig;
    std::shared_ptr<UDPServer>    _UdpServer;
};

#endif /* MENUSCENE_HPP_ */